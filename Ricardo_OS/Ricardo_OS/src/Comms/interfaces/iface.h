//abstract class of all interfaces
#ifndef IFACE_H
#define IFACE_H
#include <Arduino.h>
#include <vector>
#include <memory>
#include "interfaces.h"


class Iface{
    public:
        virtual void setup();
        virtual void send_packet(std::vector<uint8_t> &data);
        virtual void update();
        virtual ~Iface() = 0;
    protected:
        void updateSourceInterface(std::vector<uint8_t> &packet_data,std::vector<uint8_t> &destination,INTERFACE interface);

};



#endif