#include "networkManager.h"

#include "stateMachine.h"

#include "commandBuffer.h"
#include "commandHandler.h"
#include "commands.h"

#include "interfaces/interfaces.h"
#include "interfaces/radio.h"
#include "interfaces/usb.h"

#include "routingTable.h"


NetworkManager::NetworkManager(stateMachine* sm):
    usbserial(&(sm->systemstatus)),
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
    radio.update();
    usbserial.update();


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

void NetworkManager::receive_command(uint8_t iface, uint32_t command) {
    Command command_obj = Command{iface, static_cast<COMMANDS>(command)};
    commandbuffer.addCommand(command_obj);
}