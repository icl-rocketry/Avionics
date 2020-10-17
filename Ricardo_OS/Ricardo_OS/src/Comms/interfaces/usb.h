#ifndef UBS_H
#define USB_H

#include <Arduino.h>
#include "iface.h"

class Usb: public Iface{
    public:
        Usb(uint8_t address);
        void setup();
        void send_packet();
        void recieve_packet();
        

};

#endif