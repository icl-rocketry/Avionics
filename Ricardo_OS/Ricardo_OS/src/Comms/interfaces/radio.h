#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include "iface.h"
#include "SPI.h"
#include "../packets.h"
#include "Logging/systemstatus.h"
//class for lora type devices

class Radio: public Iface{
    public:
        Radio(SPIClass* spi, SystemStatus* systemstatus);
        void setup();
        void send_packet(uint8_t* txpacket_ptr,size_t packet_size);
        uint8_t* get_packet();

    private:
        SPIClass* _spi; //pointer to spi class 
        SystemStatus* _systemstatus; //pointer to system status object       
  
};



#endif