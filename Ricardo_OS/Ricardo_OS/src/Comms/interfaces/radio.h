#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include "iface.h"
#include "SPI.h"

//class for lora type devices


class Radio: public Iface{
    public:
        void setup(SPIClass& spi);
        void send_packet(uint32_t* txpacket,size_t packet_size);
        void recieve_packet(uint32_t* rxpacket);
        int8_t address;
};



#endif