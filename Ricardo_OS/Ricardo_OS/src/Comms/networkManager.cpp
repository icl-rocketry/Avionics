#include "networkManager.h"

#include "stateMachine.h"


#include "commandHandler.h"
#include "commands.h"

#include "interfaces/interfaces.h"
#include "interfaces/radio.h"
#include "interfaces/usb.h"

#include "packets.h"
#include "routingTable.h"

#include <Arduino.h>
#include <memory>
#include <vector>


NetworkManager::NetworkManager(stateMachine* sm):
    _sm(sm),
    routingtable(2,5),//preallocate size
    usbserial(&Serial,&(sm->systemstatus)),
    radio(&(sm->vspi),&(sm->systemstatus)),
    commandhandler(sm)
    
{
    //setup default routing table
    routingtable(Nodes::ROCKET) = std::vector<RoutingTableEntry>({{Interface::LOOPBACK,0},{Interface::LORA,1},{Interface::LORA,2},{Interface::CAN,1},{Interface::USBSerial,1}});
    routingtable(Nodes::GROUNDSTATION) = std::vector<RoutingTableEntry>( {{Interface::LORA,1},{Interface::LOOPBACK,0},{Interface::USBSerial,1},{Interface::LORA,2},{Interface::USBSerial,1}});
    _global_packet_buffer.reserve(5);
    _local_packet_buffer.reserve(5);
};


void NetworkManager::setup(){
    radio.setup();
    usbserial.setup();
};

void NetworkManager::update(){
    
    radio.get_packet(_global_packet_buffer); //get any new packets and place in global packet buffer
    
    usbserial.get_packet(_global_packet_buffer);
    

    process_global_packets();
   
    process_local_packets();
    
    commandhandler.update();// process any commands received
    
};

void NetworkManager::send_packet(Interface iface,std::vector<uint8_t> &data){
    switch (iface){
        case Interface::LOOPBACK:
            {

            std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>(data);

            //deserialize packet header, modify source interface and reserialize.
            PacketHeader packetheader = PacketHeader(*packet_ptr);
            //update source interface
            packetheader.src_interface = static_cast<uint8_t>(Interface::LOOPBACK);
            //serialize packet header
            std::vector<uint8_t> modified_packet_header;
            packetheader.serialize(modified_packet_header);
            //copy into original packet
            memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);


            // create new instance of shared pointer and push to global packet buffer
            //std::shared_ptr<uint8_t[]> packet_ptr(new uint8_t[len]);

            
            //*packet_ptr.reserve(data.size()); // reserve the length in the vevtor
            
            //copy data to packet_ptr
            //memcpy(*packet_ptr.data(),data.data(),data.size()); //first .get() gets the raw pointer of the vector object, the second ->get() returns the raw address of the vector
            //push onto global packet buffer for processing
            _global_packet_buffer.push_back(std::move(packet_ptr));

            break;
            }
        case Interface::LORA:
            {
            radio.send_packet(data.data(),data.size());
            break;
            }
        case Interface::USBSerial:
            {
            usbserial.send_packet(data.data(),data.size());
            break;
            }
        case Interface::CAN:
            {
            break;
            }
        default:
        //no interface selected do nothing
            break; 
    }

};

void NetworkManager::send_to_node(Nodes destination,std::vector<uint8_t> &data){
    

    uint8_t current_node = static_cast<uint8_t>(node_type);
    //get sending interface from routing table
    Interface send_interface = routingtable(current_node,static_cast<uint8_t>(destination)).gateway;
    
    if (send_interface == Interface::ERROR){
        // dump this packet as it looks like the dodgyness of highest quality
        return;
    }

    if ((send_interface == Interface::LOOPBACK) && (current_node != static_cast<uint8_t>(destination))){
        /*
                DO NOTHING... some one has messed up the routing table. 
                
                explanation: if we send this packet over the loopback it will place the packet back in the global packet buffer
                but after send we delete the pointer to clean up so the pointer will be pointing to freed memory which can lead to undefined 
                actions. Further as the destination for not equal to the source this packet will get cycled in the buffer forever which
                isnt good.
                */
    }else{
        send_packet(send_interface,data);
    };

}

int NetworkManager::get_node_type(){return static_cast<int>(node_type);};


void NetworkManager::process_global_packets(){
    if (_global_packet_buffer.size() > 0){
        
        //std::shared_ptr<uint8_t[]> curr_packet_ptr = _global_packet_buffer.front();
        
        std::unique_ptr<std::vector<uint8_t>> curr_packet = std::move(_global_packet_buffer.front()); 
  
        _global_packet_buffer.erase(_global_packet_buffer.begin()); // erase the first elemnt of global buffer
        //create temporary packet buffer object to decode packet header

        PacketHeader packetheader = PacketHeader(*curr_packet); // get vector object reference

        //get current node type

        uint8_t current_node = static_cast<uint8_t>(node_type);
        
        if (packetheader.destination != current_node){

            //forward packet to next node
            
            send_to_node(static_cast<Nodes>(packetheader.destination),*curr_packet);

            


        }else{

            if (packetheader.source == current_node){
                /*this could be because of 2 of the same node types on the same network 
                or because we are using the loopback interface but we cannot distinguish.
                 UUID or comparing source interface can distinguish.
                */

               if (packetheader.src_interface == static_cast<uint8_t>(Interface::LOOPBACK)){
                   //loopback packet
                   //delete[] curr_packet_ptr;
                   _local_packet_buffer.push_back(std::move(curr_packet));
                   //_global_packet_buffer.erase(_global_packet_buffer.begin());

               }else{
                   //more than 1 device with same node type on network.
                   //delete[] curr_packet_ptr;
                   //_global_packet_buffer.erase(_global_packet_buffer.begin());
               }
            }else{
                //YOU GOT MAIL!!!
                //place packet into local packet buffer for processing
                
                _local_packet_buffer.push_back(std::move(curr_packet));
                //_global_packet_buffer.erase(_global_packet_buffer.begin());
            }
        }
    }else{
        //nothing to process
    };
}


void NetworkManager::process_local_packets(){
    //function processes all local packets in packet buffer.

    if (_local_packet_buffer.size() > 0){
        //std::shared_ptr<uint8_t[]> curr_packet_ptr = _local_packet_buffer.front();
        std::unique_ptr<std::vector<uint8_t>> curr_packet = std::move(_local_packet_buffer.front());
        _local_packet_buffer.erase(_local_packet_buffer.begin());
        //create temporary packet buffer object to decode packet header
        PacketHeader packetheader = PacketHeader(*curr_packet);

        if (packetheader.src_interface == static_cast<uint8_t>(Interface::LOOPBACK)){
            /*erase any packets sent on loopback for now. 
            can be modified later to test this code actually works without a second board
            or even a  bin for packets we want to kill in a live system
            but then we may as well have some fun and create a death pit interface 
            */
            //delete[] curr_packet_ptr;
            

        }else{

            switch(packetheader.type){
                case static_cast<uint8_t>(packet::TELEMETRY):
                    {
                        //telemerty packets are only processed if ricardo is acting as groundstation
                        if (node_type == Nodes::GROUNDSTATION){
                            //deserialize whole packet
                            //TelemetryPacket rocket_telemetry = TelemetryPacket(curr_packet_ptr.get());

                            //copy packet data to remote rocket telemetry buffer

                            /*
                            do we then forward the telemetry packet to the laptop by sending here or do we handle it in the state?
                            it is probably a better idea to wait for a command from the laptop to send telemetry data to prevent bus collisons
                            in a more request like fashion so commands from the laptop can always be received and executed
                            */

                        //delete packet pointer object and remove from buffer
                            //delete[] curr_packet_ptr;
                            //_local_packet_buffer.erase(_local_packet_buffer.begin());
                        }else{
                            //delete packet as telemtry packets are not processed in any other state
                            //delete[] curr_packet_ptr;
                            //_local_packet_buffer.erase(_local_packet_buffer.begin());
                        }
                    }
                    break;
                case static_cast<uint8_t>(packet::COMMAND):
                    {
                        //deserialize packet
                        CommandPacket commandpacket = CommandPacket(*curr_packet);

                        //add command to command buffer
                        //add_command(static_cast<Nodes>(commandpacket.header.source),commandpacket.command);

                        Command command_obj = Command{
                            static_cast<Nodes>(commandpacket.header.source), 
                            static_cast<COMMANDS>(commandpacket.command),
                            commandpacket.arg};
                        commandhandler.addCommand(command_obj);

                        //delete packet pointer and remove from packet buffer
                        //delete[] curr_packet_ptr;
                        //_local_packet_buffer.erase(_local_packet_buffer.begin());

                        break;
                    }
                default:
                    {
                        //handle all other packet types received
                        //delete packet as somehow a packet has slipped thru and shouldnt be processed on this node
                        //delete[] curr_packet_ptr;
                       // _local_packet_buffer.erase(_local_packet_buffer.begin());  

                        break;
                    }
            };
        };

    }else{
        //nothing to process
    };

}

