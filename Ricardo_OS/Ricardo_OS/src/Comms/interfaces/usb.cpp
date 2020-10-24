#include "usb.h"

#include "iface.h"
#include "PacketSerial.h"
#include "Logging/systemstatus.h"

USB::USB(SystemStatus* systemstatus): 
    _ps()
{
    _systemstatus = systemstatus;
};

void USB::setup(){
    _ps.setStream(&Serial);
    //packet receive handler called in downlink setup 
};

void USB::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    _ps.send(data,size);
};

void USB::update(){
    _ps.update();    
};

void USB::onPacketReceived(const uint8_t* data, size_t size){
    //do something with packet - add it to command buffer 
    
};