#pragma once

#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <optional>


#include "rnp_packet.h"

using packetptr_t = std::unique_ptr<RnpPacketSerialized>;
using packetBuffer_t = std::queue<packetptr_t>;

enum class DEFAULT_INTERFACES: uint8_t{
    LOOPBACK = 0,
    USBSERIAL = 1
};

struct RnpInterfaceInfo{
    //add commmon members here
    bool state; // interface up or dowwwwnnnnn
    bool error; // in error ?? or error codes?
    virtual ~RnpInterfaceInfo(){};
};

class RnpInterface{
    public:
        RnpInterface(uint8_t id):
        _packetBuffer(nullptr),
        _id(id)
        {};
        virtual void setup() = 0;
        virtual void sendPacket(RnpPacket& data) = 0;
        virtual void send(std::vector<uint8_t>& data) = 0;
        virtual void update() = 0;
        virtual RnpInterfaceInfo* getInfo() = 0;
        virtual std::string getName() = 0;
        virtual ~RnpInterface(){};

        void setPacketBuffer(packetBuffer_t* buffer){_packetBuffer = buffer;};
        uint8_t getID(){return _id;}

    protected:
        packetBuffer_t* _packetBuffer;
        uint8_t _id; // unique id for interface 
};

