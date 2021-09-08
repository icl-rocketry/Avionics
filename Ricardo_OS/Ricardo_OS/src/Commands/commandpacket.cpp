#include "commandpacket.h"
#include "rnp_packet.h"
#include "commandHandler.h"

CommandPacket::~CommandPacket()
{};

CommandPacket::CommandPacket(uint8_t _command, uint32_t _arg):
RnpPacket(CommandHandler::serviceID,
          0,
          size()),
command(_command),
arg(_arg)
{};

CommandPacket::CommandPacket(RnpPacketSerialized& packet):
RnpPacket(packet.header)
{
    getSerializer().deserialize(*this,packet.getBody());
};

void CommandPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};