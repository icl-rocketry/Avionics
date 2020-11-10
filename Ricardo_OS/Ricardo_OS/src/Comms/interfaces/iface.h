//abstract class of all interfaces
#ifndef IFACE_H
#define IFACE_H
#include <Arduino.h>

class Iface{
    public:
        virtual void setup();
        virtual void send_packet(uint8_t* txpacket, size_t packet_len);
        virtual void get_packet(std::vector<uint8_t*> *buf);//update function 
    
    

};

#endif