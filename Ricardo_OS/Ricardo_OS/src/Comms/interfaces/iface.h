//abstract class of all interfaces
#ifndef IFACE_H
#define IFACE_H
#include <Arduino.h>

class Iface{
    public:
        Iface(uint8_t address);
        virtual ~Iface() = 0;//pure virtual destructor

        virtual void setup();
        virtual void send_packet(uint8_t* txpacket, size_t packet_len);
        virtual void recieve_packet(uint8_t* rxpacket);

        virtual void update();//update function 
        
        uint8_t _address;//address of interface
    

};

#endif