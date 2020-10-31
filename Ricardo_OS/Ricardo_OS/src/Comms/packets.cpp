//packet.cpp
#include "packets.h"

PacketHeader::PacketHeader() {}
PacketHeader::PacketHeader(uint8_t packet_type) : type{packet_type} {}

PacketHeader::PacketHeader(uint8_t* data, uint8_t size) {
	int step = 0;
	for (int i = 0; i < size; ++i) {
		const auto b = data[i];
		switch (step) {
		case 0:
			if (data[i] != 0xAF) // Look for the start byte in data
			{
				step = 0;
				continue;
			}
			this->start_byte = b;
			step++;
			break;
		case 1:
			this->type = b;
			step++;
			break;
		case 2:
			this->source = b;
			step++;
			break;
		case 3:
			this->destination = b;
			step++;
			break;
		case 4:
			this->packet_len = b;
			step++;
			break;
		case 5:
			this->checksum = b;
			step++;
			break;
		}
	}
	
}

PacketHeader::~PacketHeader() {}

void PacketHeader::serialize(std::vector<uint8_t>& buf) {
    buf.push_back(start_byte);
	buf.push_back(type);
    buf.push_back(source);
	buf.push_back(destination);
	buf.push_back(packet_len);
    buf.push_back(checksum);
}


void TelemetryPacket::serialize(std::vector<uint8_t>& buf) {
	header.serialize(buf);

	// TODO: Serialize floats into bytes
	// TODO: Compute checksums
}

void CommandPacket::serialize(std::vector<uint8_t>& buf) {
	header.serialize(buf);
	buf.push_back(command);
	// TODO: Compute checksums
}

void DetailedAllPacket::serialize(std::vector<uint8_t>& buf) {
	header.serialize(buf);

	// TODO: Serialize floats into bytes
	// TODO: Compute checksums
}

DetailedAllPacket::DetailedAllPacket(uint8_t* data, uint8_t size) {
	// TODO: Implement deserializer
}

CommandPacket::CommandPacket(uint8_t* data, uint8_t size) {
	// TODO: Implement deserializer
}

TelemetryPacket::TelemetryPacket(uint8_t* data, uint8_t size) {
	// TODO: Implement deserializer
}