#ifndef UBS_H
#define USB_H

#include <Arduino.h>
#include "iface.h"

#include "PacketSerial.h"

class USB: public Iface{
    public:
        USB();
        void setup();
        void send_packet(uint8_t* data , size_t size);
        void recieve_packet();
        void update();

        //packet handler function for packet serial
        void onSerialPacketRecieved(uint8_t* data, size_t size);

        PacketSerial _ps; //packetserial object


};

#endif