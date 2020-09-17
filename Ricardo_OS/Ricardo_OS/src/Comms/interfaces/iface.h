//parent class of all interfaces
#include <Arduino.h>
#ifndef IFACE_H
#define IFACE_H


class Iface{
    public:
        virtual void setup();
        virtual void send_packet(uint32_t &packet,int size);
        virtual void on_recieve();

};

#endif