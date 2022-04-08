#include "hitlpacket.h"

#include "rnp_packet.h"

#include <vector>



PickleRickSensorsPacket::~PickleRickSensorsPacket()
{};

PickleRickSensorsPacket::PickleRickSensorsPacket():
RnpPacket(0, //no hitl default
          static_cast<uint8_t>(HITL_PACKET_TYPES::PICKLE_RICK_SENSORS),
          size())
{};

PickleRickSensorsPacket::PickleRickSensorsPacket(const RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    getSerializer().deserialize(*this,packet.getBody());
};

void PickleRickSensorsPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};