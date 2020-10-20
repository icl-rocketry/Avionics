#ifndef UBS_H
#define USB_H

#include <Arduino.h>
#include "iface.h"

#include "PacketSerial.h"

class Usb: public Iface{
    public:
        Usb(uint8_t address);
        void setup();
        void send_packet(uint8_t* data , size_t size);
        void recieve_packet();
        void onSerialPacketRecieved(uint8_t* data, size_t size);

        PacketSerial _ps; //packetserial object


};

#endif