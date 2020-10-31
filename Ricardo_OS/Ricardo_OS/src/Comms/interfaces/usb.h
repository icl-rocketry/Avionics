#ifndef USBSEERIAL_H
#define USBSEERIAL_H

#include <Arduino.h>
#include "iface.h"

#include "PacketSerial.h"

#include "Logging/systemstatus.h"



class USB: public Iface{

    public:
        USB(SystemStatus* systemstatus);
        void setup();
        void send_packet(uint8_t* data , size_t size);
        bool update();

        //packet handler function for packet serial
        void onPacketReceived(const uint8_t* data, size_t size);

        PacketSerial _ps; //packetserial object
    private:

        SystemStatus* _systemstatus;


};


#endif