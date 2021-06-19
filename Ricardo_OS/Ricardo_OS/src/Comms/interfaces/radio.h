#ifndef RADIO_H
#define RADIO_H
#include <Arduino.h>
#include "iface.h"
#include "SPI.h"
#include "../packets.h"
#include "Storage/systemstatus.h"

#include <memory>
#include <vector>
//class for lora type devices

class Radio: public Iface{
    public:
        Radio(SPIClass* spi, SystemStatus* systemstatus);
        void setup();
        void send_packet(uint8_t* txpacket_ptr,size_t packet_size);
        void get_packet(std::vector<std::unique_ptr<std::vector<uint8_t>>> &buf);

        int16_t get_rssi();

        float get_snr();


    private:
        
        SPIClass* _spi; //pointer to spi class 
        SystemStatus* _systemstatus; //pointer to system status object     
        int packetSize;  
        int index;
        std::vector<uint8_t> packet;

        uint64_t prev_time;
  
};



#endif