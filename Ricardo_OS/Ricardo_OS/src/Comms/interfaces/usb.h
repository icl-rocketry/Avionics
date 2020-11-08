#ifndef USBSEERIAL_H
#define USBSEERIAL_H

#include <Arduino.h>
#include "iface.h"

#include "../packets.h"

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

        //byte array to store first 8 bytes of incoming packet for decoding
        uint8_t _tmp_packet_data[8];
        //pointer to packet header object
        PacketHeader* _packetHeader_ptr = nullptr;
        //counter for timeout to prevent blocking of serial port
        uint8_t _timeoutCounter;
        


};


#endif