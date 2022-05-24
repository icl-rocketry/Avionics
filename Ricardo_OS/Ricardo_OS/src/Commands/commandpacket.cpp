#include "commandpacket.h"
#include "rnp_packet.h"
#include "commandHandler.h"

SimpleCommandPacket::~SimpleCommandPacket()
{};

SimpleCommandPacket::SimpleCommandPacket(uint8_t _command, uint32_t _arg):
RnpPacket(CommandHandler::serviceID,
          static_cast<uint8_t>(CommandPacket::TYPES::SIMPLE),
          size()),
command(_command),
arg(_arg)
{};

SimpleCommandPacket::SimpleCommandPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void SimpleCommandPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};


MagCalCommandPacket::~MagCalCommandPacket()
{};

MagCalCommandPacket::MagCalCommandPacket(uint8_t _command):
RnpPacket(CommandHandler::serviceID,
          static_cast<uint8_t>(CommandPacket::TYPES::MAGCAL),
          size()),
command(_command)
{};

MagCalCommandPacket::MagCalCommandPacket(const RnpPacketSerialized& packet):
RnpPacket(packet,size())
{
    getSerializer().deserialize(*this,packet.getBody());
};

void MagCalCommandPacket::serialize(std::vector<uint8_t>& buf){
    RnpPacket::serialize(buf);
	size_t bufsize = buf.size();
	buf.resize(bufsize + size());
	std::memcpy(buf.data() + bufsize,getSerializer().serialize(*this).data(),size());
};

Eigen::Matrix3f MagCalCommandPacket::getA() 
{
    return Eigen::Matrix3f{{A11,A12,A13},{A21,A22,A23},{A31,A32,A33}};
};

Eigen::Vector3f MagCalCommandPacket::getB() 
{
    return Eigen::Vector3f{{b1,b2,b3}};
};


command_t CommandPacket::getCommand(const RnpPacketSerialized& packet) 
{
    const size_t headerSize = packet.header.size();
    command_t commandID;
    std::memcpy(&commandID,packet.packet.data() + headerSize,sizeof(command_t)); // extract command id from any command packet
    return commandID;
}
