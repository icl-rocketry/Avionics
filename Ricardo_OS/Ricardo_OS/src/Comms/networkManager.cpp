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


};

void NetworkManager::send_packet(Interface iface,uint8_t* data, size_t len){
    switch (iface){
        case Interface::LOOPBACK:
            //nothing here yet but can be developed later
            break;
        case Interface::LORA:
            
            break;
        case Interface::USBSerial:
            
            break;
        case Interface::CAN:
            
            break;
        default:
        //no interface selected do nothing
            break; 
    }



};





void NetworkManager::process_global_packets(){
    if (_global_packet_buffer.size()>0){
        uint8_t* curr_packet_ptr = _global_packet_buffer.front();
        //create temporary packet buffer object to decode packet header
        PacketHeader packetheader = PacketHeader(curr_packet_ptr,PacketHeader::header_size);

        //get current node type
        uint8_t current_node = static_cast<uint8_t>(node_type);
        if (packetheader.destination != current_node){
            //forward packet to next node
            //get sending interface from routing table
            Interface send_interface = routingtable[current_node][packetheader.destination].gateway;
            //forward packet to correct interface to send to next node in network
            send_packet(send_interface,curr_packet_ptr,static_cast<size_t>(packetheader.packet_len));
            //remove packet_ptr from buffer as packet has been processed - this is dangerous. 
            //if an exception occurs anywhere after here we will get memory leaks.
            // Must change to shared ptrs
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
                   delete[] curr_packet_ptr;
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




void NetworkManager::receive_command(Interface iface, uint32_t command) {
    Command command_obj = Command{iface, static_cast<COMMANDS>(command)};
    commandbuffer.addCommand(command_obj);
}

void NetworkManager::clear_buffer(std::vector<uint8_t*>* buf){
    while (buf->size() > 0){
        uint8_t* packet_ptr = buf->front();
        delete[] packet_ptr;
        buf->erase(buf->begin());

    };
}