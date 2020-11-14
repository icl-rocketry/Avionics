#include "networkManager.h"

#include "stateMachine.h"

#include "commandBuffer.h"
#include "commandHandler.h"
#include "commands.h"

#include "interfaces/interfaces.h"
#include "interfaces/radio.h"
#include "interfaces/usb.h"

#include "packets.h"
#include "routingTable.h"

#include <memory>


NetworkManager::NetworkManager(stateMachine* sm):
    usbserial(&Serial,&(sm->systemstatus)),
    radio(&(sm->vspi),&(sm->systemstatus)),
    commandbuffer(),
    commandhandler(sm, &commandbuffer)
    
{
    _sm = sm;
};


void NetworkManager::setup(){
    radio.setup();
    usbserial.setup();
};

void NetworkManager::update(){
    radio.get_packet(&_global_packet_buffer); //get any new packets and place in global packet buffer
    usbserial.get_packet(&_global_packet_buffer);

    process_global_packets();
    process_local_packets();


};

void NetworkManager::send_packet(Interface iface,uint8_t* data, size_t len){
    switch (iface){
        case Interface::LOOPBACK:
            {
            //dump packet back onto global packet buffer - used for debuging purposes
            //_global_packet_buffer.push_back(data);

            break;
            }
        case Interface::LORA:
            {
            radio.send_packet(data,len);
            break;
            }
        case Interface::USBSerial:
            {
            usbserial.send_packet(data,len);
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





void NetworkManager::process_global_packets(){
    if (_global_packet_buffer.size() > 0){
        std::shared_ptr<uint8_t> curr_packet_ptr = _global_packet_buffer.front();
        //create temporary packet buffer object to decode packet header
        PacketHeader packetheader = PacketHeader(curr_packet_ptr.get());

        //get current node type
        uint8_t current_node = static_cast<uint8_t>(node_type);
        
        if (packetheader.destination != current_node){
            //forward packet to next node
            //get sending interface from routing table
            Interface send_interface = routingtable[current_node][packetheader.destination].gateway;
            
            if (send_interface == Interface::LOOPBACK){
                /*
                DO NOTHING... some one has messed up the routing table. 
                
                explanation: if we send this packet over the loopback it will place the packet back in the global packet buffer
                but after send we delete the pointer to clean up so the pointer will be pointing to freed memory which can lead to undefined 
                actions. Further as the destination for not equal to the source this packet will get cycled in the buffer forever which
                isnt good.
                */

            }else{
                //forward packet to correct interface to send to next node in network
                send_packet(send_interface,curr_packet_ptr.get(),static_cast<size_t>(packetheader.packet_len));
            }
            //delete packet pointer as we no longer need it
            //delete[] curr_packet_ptr;
            _global_packet_buffer.erase(_global_packet_buffer.begin());

        }else{
            if (packetheader.source == current_node){
                /*this could be because of 2 of the same node types on the same network 
                or because we are using the loopback interface but we cannot distinguish.
                 UUID or comparing source interface can distinguish.
                */
               if (packetheader.src_interface == static_cast<uint8_t>(Interface::LOOPBACK)){
                   //loopback packet
                   //delete[] curr_packet_ptr;
                   _local_packet_buffer.push_back(curr_packet_ptr);
                   _global_packet_buffer.erase(_global_packet_buffer.begin());

               }else{
                   //more than 1 device with same node type on network.
                   //delete[] curr_packet_ptr;
                   _global_packet_buffer.erase(_global_packet_buffer.begin());
               }
            }else{
                //YOU GOT MAIL!!!
                //place packet into local packet buffer for processing
                _local_packet_buffer.push_back(curr_packet_ptr);
                _global_packet_buffer.erase(_global_packet_buffer.begin());
            }
        }
    }else{
        //nothing to process
    };
}

void NetworkManager::process_local_packets(){
    //function processes all local packets in packet buffer. each delete is explictly written
    //so it is obvious where each packet pointer is deleted and removed from buffer.
    //need to be careful we dont call delete twice tho - this is UNDEFINED BEHAVIOUR
    //this will all hopefully be cleaned up when we transition to smart pointers

    if (_local_packet_buffer.size() > 0){
        std::shared_ptr<uint8_t> curr_packet_ptr = _local_packet_buffer.front();
        //create temporary packet buffer object to decode packet header
        PacketHeader packetheader = PacketHeader(curr_packet_ptr.get());

        if (packetheader.src_interface == static_cast<uint8_t>(Interface::LOOPBACK)){
            /*erase any packets sent on loopback for now. 
            can be modified later to test this code actually works without a second board
            or even a dynamic bin for packets we want to kill in a live system
            but then we may as well have some fun and create a death pit interface 
            */
            //delete[] curr_packet_ptr;
            _local_packet_buffer.erase(_local_packet_buffer.begin());

        }else{

            switch(packetheader.type){
                case static_cast<uint8_t>(packet::TELEMETRY):
                    {
                        //telemerty packets are only processed if ricardo is acting as groundstation
                        if (node_type == Nodes::GROUNDSTATION){
                            //deserialize whole packet
                            TelemetryPacket rocket_telemetry = TelemetryPacket(curr_packet_ptr.get());

                            //copy packet data to remote rocket telemetry buffer

                            /*
                            do we then forward the telemetry packet to the laptop by sending here or do we handle it in the state?
                            it is probably a better idea to wait for a command from the laptop to send telemetry data to prevent bus collisons
                            in a more request like fashion so commands from the laptop can always be received and executed
                            */

                        //delete packet pointer object and remove from buffer
                            //delete[] curr_packet_ptr;
                            _local_packet_buffer.erase(_local_packet_buffer.begin());
                        }else{
                            //delete packet as telemtry packets are not processed in any other state
                            //delete[] curr_packet_ptr;
                            _local_packet_buffer.erase(_local_packet_buffer.begin());
                        }
                    }
                    break;
                case static_cast<uint8_t>(packet::COMMAND):
                    {
                        //deserialize packet
                        CommandPacket commandpacket = CommandPacket(curr_packet_ptr.get());

                        //add command to command buffer
                        add_command(static_cast<Nodes>(commandpacket.header.source),commandpacket.command);
                        //delete packet pointer and remove from packet buffer
                        //delete[] curr_packet_ptr;
                        _local_packet_buffer.erase(_local_packet_buffer.begin());

                        break;
                    }
                default:
                    {
                        //handle all other packet types received
                        //delete packet as somehow a packet has slipped thru and shouldnt be processed on this node
                        //delete[] curr_packet_ptr;
                        _local_packet_buffer.erase(_local_packet_buffer.begin());  

                        break;
                    }
            };
        };

    }else{
        //nothing to process
    };

}



void NetworkManager::add_command(Nodes source_node, uint32_t command) {
    Command command_obj = Command{source_node, static_cast<COMMANDS>(command)};
    commandbuffer.addCommand(command_obj);
}

/*

void NetworkManager::clear_buffer(std::vector<uint8_t*>* buf){
    while (buf->size() > 0){
        uint8_t* packet_ptr = buf->front();
        delete[] packet_ptr;
        buf->erase(buf->begin());

    };
}
*/