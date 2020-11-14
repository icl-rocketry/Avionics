#ifndef USBSEERIAL_H
#define USBSEERIAL_H

#include <Arduino.h>
#include "iface.h"

#include "../packets.h"

#include "Logging/systemstatus.h"

#include <memory>



class USB: public Iface{

    public:
        USB(Stream* stream,SystemStatus* systemstatus);
        void setup();
        void send_packet(uint8_t* data , size_t size);
        void get_packet(std::vector<std::shared_ptr<uint8_t>> *buf);



    private:
        Stream* _stream; // pointer to stream interface
        SystemStatus* _systemstatus; //pointer to system status object

        bool _incompletePacketReceived;
        uint8_t _firstByte;

        //byte array to store first 8 bytes of incoming packet for decoding
        uint8_t _tmp_packet_data[8];
        //pointer to packet header object
        //PacketHeader* _packetHeader_ptr = nullptr;
        
        //Packet size
        uint32_t _packet_len;
        //expected size of header
        uint8_t _packetHeader_size = PacketHeader::header_size;
        //counter for timeout to prevent blocking of serial port
        uint8_t _timeoutCounter;
        


};


#endif