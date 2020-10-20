#include "usb.h"
#include "iface.h"
#include "PacketSerial.h"


Usb::Usb(uint8_t address): 
    Iface(address),
    _ps()
    {

};

void Usb::setup(){
    _ps.setStream(&Serial);
};

void Usb::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    _ps.send(data,size);
};

void Usb::recieve_packet(){

};

void Usb::onSerialPacketRecieved(uint8_t* data, size_t size){
    //do something with packet - add it to command buffer 
};