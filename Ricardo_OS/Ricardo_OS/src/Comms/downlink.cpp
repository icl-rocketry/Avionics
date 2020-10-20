#include "downlink.h"

#include "stateMachine.h"

#include "commandBuffer.h"
#include "commandHandler.h"
#include "commands.h"

#include "interfaces/radio.h"
#include "interfaces/usb.h"

Downlink::Downlink(stateMachine* sm):
    commandhandler(sm, &buffer),
    radio(1),
    usb(2)
{
    _sm = sm;
};


void Downlink::setup(){
    radio.setup();
    usb.setup();
    
};

void Downlink::update(){
    radio.update();
    usb.update();


};

void Downlink::send_data(uint8_t iface,uint8_t* data, size_t len){
    

};

void Downlink::receive_command(uint8_t iface, uint32_t command) {
    Command command_obj = Command(iface, static_cast<COMMANDS>(command));
    buffer.addCommand(command_obj);
}