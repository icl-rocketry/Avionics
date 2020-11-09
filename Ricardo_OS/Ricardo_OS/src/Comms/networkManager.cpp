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
    update_buffer(&radio,&_global_packet_buffer);
    update_buffer(&usbserial,&_global_packet_buffer);

    process_global_packets(&_global_packet_buffer);


};

void NetworkManager::send_data(Interface iface,uint8_t* data, size_t len){
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



void NetworkManager::update_buffer(Iface* iface,std::vector<uint8_t*>* buf){
    uint8_t* data_ptr = iface->get_packet(); // get any packets from interface
    if (data_ptr != nullptr) //check if a packet has been returned
        buf->push_back(data_ptr);
    else{
        //do nothing cos no data returned
    };
}

void NetworkManager::process_global_packets(std::vector<uint8_t*>* global_buf){
    if (global_buf->size()>0){
        uint8_t* curr_packet_ptr = global_buf->front();
        //process some stuff
        
        //delete array pointer prevent memory leak
        delete[] curr_packet_ptr;
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