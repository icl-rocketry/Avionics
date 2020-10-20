#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include "iface.h"
#include "SPI.h"
#include "../packets.h"
//class for lora type devices


class Radio: public Iface{
    public:
        Radio(SPIClass* spi);
        void setup();
        void send_packet(uint8_t* txpacket_ptr,size_t packet_size);
        void recieve_packet(uint8_t* rxpacket_ptr);
        void update();
    private:
        SPIClass* _spi; //pointer to spi class        
  
};



#endif