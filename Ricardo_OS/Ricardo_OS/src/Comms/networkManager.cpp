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
#include "flags.h"


NetworkManager::NetworkManager(stateMachine* sm):
    
    usbserial(&Serial,&(sm->systemstatus)),
    radio(&(sm->vspi),&(sm->systemstatus)),
    _sm(sm),
    routingtable(2,5),//preallocate size
    commandhandler(sm)
    
{
   
    routingtable(NODES::ROCKET) = std::vector<RoutingTableEntry>({{INTERFACE::LOOPBACK,0},{INTERFACE::LORA,1},{INTERFACE::LORA,2},{INTERFACE::CAN,1},{INTERFACE::USBSerial,1}});
    routingtable(NODES::GROUNDSTATION) = std::vector<RoutingTableEntry>( {{INTERFACE::LORA,1},
                                                                          {INTERFACE::LOOPBACK,0},
                                                                          {INTERFACE::USBSerial,1},
                                                                          {INTERFACE::LORA,2},
                                                                          {INTERFACE::USBSerial,1}
                                                                          });

    _packet_buffer.reserve(5);
};


void NetworkManager::setup(){

    radio.setup();
    usbserial.setup();
};

void NetworkManager::update(){
    
    radio.get_packet(_packet_buffer);
    
    usbserial.get_packet(_packet_buffer);
    

    process_packets();
    
    commandhandler.update();// process any commands received
    
};

void NetworkManager::send_packet(INTERFACE iface,std::vector<uint8_t> &data){
    switch (iface){
        case INTERFACE::LOOPBACK:
            {

            std::unique_ptr<std::vector<uint8_t>> packet_ptr = std::make_unique<std::vector<uint8_t>>(data);
            //deserialize packet header, modify source interface and reserialize.
            PacketHeader packetheader = PacketHeader(*packet_ptr);
            //update source interface
            packetheader.src_interface = static_cast<uint8_t>(INTERFACE::LOOPBACK);
            //serialize packet header
            std::vector<uint8_t> modified_packet_header;
            packetheader.serialize(modified_packet_header);
            //copy into original packet
            memcpy((*packet_ptr).data(),modified_packet_header.data(),packetheader.header_len);
            //push onto global packet buffer for processing
            _packet_buffer.push_back(std::move(packet_ptr));

            break;
            }
        case INTERFACE::LORA:
            {
            radio.send_packet(data.data(),data.size());
            break;
            }
        case INTERFACE::USBSerial:
            {
            usbserial.send_packet(data.data(),data.size());
            break;
            }
        case INTERFACE::CAN:
            {
            break;
            }
        default:
        //interface invalid dump packet
            break; 
    }

};

void NetworkManager::send_to_node(NODES destination,std::vector<uint8_t> &data){
    
   
    uint8_t current_node = static_cast<uint8_t>(_nodeType);
    //get sending interface from routing table
    INTERFACE send_interface = routingtable(current_node,static_cast<uint8_t>(destination)).gateway;
    //Serial.println((int)send_interface);
 
    if (send_interface == INTERFACE::ERROR){
        // dump this packet as it looks like the dodgyness of highest quality
        return;
    }

    if ((send_interface == INTERFACE::LOOPBACK) && (current_node != static_cast<uint8_t>(destination))){
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

uint8_t NetworkManager::getNodeType(){return static_cast<uint8_t>(_nodeType);};


void NetworkManager::process_packets(){
    if (_packet_buffer.size() > 0){
        
        
        std::unique_ptr<std::vector<uint8_t>> curr_packet = std::move(_packet_buffer.front()); 
  
        _packet_buffer.erase(_packet_buffer.begin()); // erase the first elemnt of global buffer

        //decode packet header 
        PacketHeader packetheader = PacketHeader(*curr_packet); 

        //get current node type
        uint8_t current_node = static_cast<uint8_t>(_nodeType);
        
        if (packetheader.destination != current_node){

            //forward packet to next node
            send_to_node(static_cast<NODES>(packetheader.destination),*curr_packet);

        }else{

            if (packetheader.source == current_node){
               if (packetheader.src_interface == static_cast<uint8_t>(INTERFACE::LOOPBACK)){
                   /*erase any packets sent on loopback for now. 
                    can be modified later to test this code actually works without a second board
                    or even a  bin for packets we want to kill in a live system
                    but then we may as well have some fun and create a death pit interface 
                    */
               }
            }else{
                //LOCAL PACKET LOGIC
                //process packets meant for this node
                switch(packetheader.type){
                    case static_cast<uint8_t>(packet::COMMAND):
                        {
                            //deserialize packet
                            CommandPacket commandpacket = CommandPacket(*curr_packet);
                            Command command_obj = Command{
                                static_cast<NODES>(commandpacket.header.source), 
                                static_cast<COMMANDS>(commandpacket.command),
                                commandpacket.arg,
                                commandpacket.header.uid
                                };
                            commandhandler.addCommand(command_obj);

                            break;
                        }
                    default:
                        {
                            //dump any packets we dont care about
                            break;
                        }
                }
            }
        }
    }
}

