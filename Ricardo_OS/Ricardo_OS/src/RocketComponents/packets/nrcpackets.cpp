#include "nrcpackets.h"

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

#include <vector>


NRCStatePacket::~NRCStatePacket()
{};

NRCStatePacket::NRCStatePacket(uint8_t destinationService):
RnpPacket(destinationService,
          static_cast<uint8_t>(NRCPacket::TYPES::NRC_STATE),
          size())
{};

NRCStatePacket::NRCStatePacket(const RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    getSerializer().deserialize(*this,packet.getBody());
};

void NRCStatePacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};