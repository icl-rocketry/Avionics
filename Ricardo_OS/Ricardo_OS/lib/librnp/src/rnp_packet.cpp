#include "rnp_packet.h"
#include "rnp_header.h"

#include <vector>


RnpPacket::~RnpPacket()
{};

RnpPacket::RnpPacket(uint8_t packetService, uint8_t packetType, uint16_t packetSize):
header(packetService,packetType,packetSize)
{};

RnpPacket::RnpPacket(RnpHeader _header):
header(_header)
{};


void RnpPacket::serialize(std::vector<uint8_t>& buf){
    header.serialize(buf);
}

RnpPacketSerialized::~RnpPacketSerialized()
{};

RnpPacketSerialized::RnpPacketSerialized(const std::vector<uint8_t> &bytes):
RnpPacket(RnpHeader(bytes)),
packet(bytes)
{};

void RnpPacketSerialized::reserializeHeader(){
    std::vector<uint8_t> header_serialized;
    header.serialize(header_serialized);
    std::memcpy(packet.data(),header_serialized.data(),header_serialized.size()); // replace original header in serialized packet
}

void RnpPacketSerialized::serialize(std::vector<uint8_t>& buf) {
    reserializeHeader();
    size_t bufsize = buf.size();
    buf.resize(bufsize + packet.size());
    std::memcpy(buf.data() + bufsize, packet.data(),packet.size());
};

std::vector<uint8_t> RnpPacketSerialized::getBody()const { 
    return std::vector<uint8_t> {packet.begin() + header.size(),packet.end()};
}

size_t RnpPacketSerialized::getBodySize()const {
    return (packet.size() < header.size()) ? (0) : (packet.size() - header.size()); //ensure we dont return a negative number 
}