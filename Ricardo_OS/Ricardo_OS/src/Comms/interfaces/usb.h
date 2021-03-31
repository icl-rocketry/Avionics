#ifndef USBSEERIAL_H
#define USBSEERIAL_H

#include <Arduino.h>
#include "iface.h"

#include "../packets.h"

#include "Logging/systemstatus.h"

#include <memory>
#include <vector>
#include <array>



class USB: public Iface{

    public:
        USB(Stream* stream,SystemStatus* systemstatus);
        void setup();
        void send_packet(uint8_t* data , size_t size);
        void get_packet(std::vector<std::unique_ptr<std::vector<uint8_t>>> &buf);



    private:
        Stream* _stream; // pointer to stream interface
        SystemStatus* _systemstatus; //pointer to system status object

        bool _incompletePacketReceived;
        uint8_t _firstByte;

        //byte array to store first bytes of incoming packet for decoding
        //uint8_t _tmp_packet_data[PacketHeader::header_size()];
        std::vector<uint8_t> _tmp_packet_data;
        //pointer to packet header object
        //PacketHeader* _packetHeader_ptr = nullptr;
        
        //Packet size
        uint32_t _packet_len;
        //expected size of header
        const uint8_t _packetHeader_size = PacketHeader::header_size();
        //decoded header size
        uint8_t _header_len;
        //size of data including header
        uint32_t _total_len;
        //counter for timeout to prevent blocking of serial port
        uint8_t _timeoutCounter;
        


};


#endif