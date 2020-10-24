#include "downlink.h"

#include "stateMachine.h"

#include "commandBuffer.h"
#include "commandHandler.h"
#include "commands.h"

#include "interfaces/radio.h"
#include "interfaces/usb.h"

Downlink::Downlink(stateMachine* sm):
    usbserial(),
    radio(&(sm->vspi)),
    commandbuffer(),
    commandhandler(sm, &commandbuffer)
    
{
    _sm = sm;
};


void Downlink::setup(){
    radio.setup();
    usbserial.setup();
};

void Downlink::update(){
    radio.update();
    usbserial.update();


};

void Downlink::send_data(uint8_t iface,uint8_t* data, size_t len){
    switch (iface){
        case 1:
            break;
        default:
        //no interface selected do nothing
            break; 
    }



};

void Downlink::receive_command(uint8_t iface, uint32_t command) {
    Command command_obj = Command{iface, static_cast<COMMANDS>(command)};
    commandbuffer.addCommand(command_obj);
}