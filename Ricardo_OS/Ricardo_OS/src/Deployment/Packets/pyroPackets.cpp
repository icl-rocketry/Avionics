#include "pyroPackets.h"

#include "rnp_networkmanager.h"
#include "rnp_packet.h"

#include <vector>


uint8_t PyroPacket::getPyroID(const RnpPacketSerialized& packet) 
{
    const size_t headerSize = packet.header.size();
    uint8_t pyroID;
    std::memcpy(&pyroID,packet.packet.data() + headerSize,sizeof(uint8_t)); // extract command id from any command packet
    return pyroID;
}

PyroInfoPacket::~PyroInfoPacket()
{};

PyroInfoPacket::PyroInfoPacket():
RnpPacket(static_cast<uint8_t>(DEFAULT_SERVICES::PYRO),
          0,
          size())
{};

PyroInfoPacket::PyroInfoPacket(const RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    getSerializer().deserialize(*this,packet.getBody());
};

void PyroInfoPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};