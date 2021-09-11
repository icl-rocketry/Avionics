#include "TelemetryPacket.h"

#include "rnp_networkmanager.h"
#include "rnp_packet.h"

#include <vector>

// TelemetryPacket::TelemetryPacket() {}
// TelemetryPacket::~TelemetryPacket() {}

// void TelemetryPacket::serialize(std::vector<uint8_t>& buf) const{
// 	header.serialize(buf);

// 	std::vector<uint8_t> serialized_data = getSerializer().serialize(*this);
// 	size_t bufsize = buf.size();
// 	buf.resize(buf.size() + serialized_data.size());
// 	memcpy(buf.data() + bufsize,serialized_data.data(),serialized_data.size());

// };

// TelemetryPacket::TelemetryPacket(const std::vector<uint8_t> &data):header(data) {
// 	//header = PacketHeader(data); // Deserialize the header
// 	//shift data buffer to end of header to get body of packet
//     //const uint8_t* body = data + header.header_len;
// 	//initalize vector from c array 
// 	//std::vector<uint8_t> buffer(body ,body + packet_size());
// 	std::vector<uint8_t> packet_body;
// 	packet_body.resize(data.size()-header.header_len);
// 	memcpy(packet_body.data(),data.data() + header.header_len,packet_body.size());

// 	getSerializer().deserialize(*this, packet_body);
// };


TelemetryPacket::~TelemetryPacket()
{};

TelemetryPacket::TelemetryPacket():
RnpPacket(static_cast<uint8_t>(DEFAULT_SERVICES::TELEMETRY),
          0,
          size())
{};

TelemetryPacket::TelemetryPacket(RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    getSerializer().deserialize(*this,packet.getBody());
};

void TelemetryPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};