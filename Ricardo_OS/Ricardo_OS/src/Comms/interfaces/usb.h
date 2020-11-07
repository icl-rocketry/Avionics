#ifndef USBSEERIAL_H
#define USBSEERIAL_H

#include <Arduino.h>
#include "iface.h"

#include "PacketSerial.h"

#include "Logging/systemstatus.h"



class USB: public Iface{

    public:
        USB(Stream* stream,SystemStatus* systemstatus);
        void setup();
        void send_packet(uint8_t* data , size_t size);
        uint8_t* get_packet();



    private:
        Stream* _stream; // pointer to stream interface
        SystemStatus* _systemstatus; //pointer to system status object

        bool _incompletePacketReceived;
        uint8_t _firstByte;


};


#endif