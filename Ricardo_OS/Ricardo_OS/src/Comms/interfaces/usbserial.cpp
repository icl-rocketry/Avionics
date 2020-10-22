#include "usbserial.h"

#include "iface.h"
#include "PacketSerial.h"


USBserial::USBserial(): 
    _ps()
{
};

void USBserial::setup(){
    _ps.setStream(&Serial);
};

void USBserial::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    _ps.send(data,size);
};

void USBserial::recieve_packet(){

};

void USBserial::update(){
    _ps.update();
};

void USBserial::onSerialPacketRecieved(uint8_t* data, size_t size){
    //do something with packet - add it to command buffer 
};