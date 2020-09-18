//parent class of all interfaces
#ifndef IFACE_H
#define IFACE_H
#include <Arduino.h>

class Iface{
    public:
        virtual void setup(int8_t address);
        virtual void send_packet(uint32_t* txpacket, size_t packet_len);
        virtual void recieve_packet(uint32_t* rxpacket);
        int8_t address;
    

};

#endif