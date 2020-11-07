//abstract class of all interfaces
#ifndef IFACE_H
#define IFACE_H
#include <Arduino.h>

class Iface{
    public:
        virtual void setup();
        virtual void send_packet(uint8_t* txpacket, size_t packet_len);
        virtual uint8_t* get_packet();//update function 
    
    

};

#endif