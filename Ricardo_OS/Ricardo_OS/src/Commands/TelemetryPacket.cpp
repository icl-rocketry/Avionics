#include "TelemetryPacket.h"

#include "rnp_networkmanager.h"
#include "rnp_packet.h"

#include <vector>



TelemetryPacket::~TelemetryPacket()
{};

TelemetryPacket::TelemetryPacket():
RnpPacket(0,
          101,
          size())
{};

TelemetryPacket::TelemetryPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void TelemetryPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};