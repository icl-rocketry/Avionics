#ifndef USBSEERIAL_H
#define USBSEERIAL_H

#include <Arduino.h>
#include "iface.h"

#include "PacketSerial.h"

class USB: public Iface{

    public:
        USB();
        void setup();
        void send_packet(uint8_t* data , size_t size);
        void update();

        //packet handler function for packet serial
        void onPacketReceived(const uint8_t* data, size_t size);

        PacketSerial _ps; //packetserial object


};

#endif