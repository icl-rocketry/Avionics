#include "usb.h"

#include "iface.h"
#include "PacketSerial.h"


USB::USB(): 
    _ps()
{
};

void USB::setup(){
    _ps.setStream(&Serial);
};

void USB::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    _ps.send(data,size);
};

void USB::recieve_packet(){

};

void USB::update(){
    _ps.update();
};

void USB::onSerialPacketRecieved(uint8_t* data, size_t size){
    //do something with packet - add it to command buffer 
};