#include "usb.h"

#include "iface.h"
#include "PacketSerial.h"
#include "Logging/systemstatus.h"

USB::USB(SystemStatus* systemstatus) 
{
    _systemstatus = systemstatus;
};

void USB::setup(){

};

void USB::send_packet(uint8_t* data, size_t size){ // From RICARDO to USB
    //maybe check if we can write using serial.avalibalewrite not sure what action we would take 
    //if we couldnt write though
    Serial.write(data,size);
};

bool USB::update(){
    
    int buf_size = Serial.available();

    if (buf_size > 0){
        //find and process any and all packets

    }else{
        return false;

    };
};   
