//packet.cpp
#include "packets.h"

PacketHeader::PacketHeader() {}
PacketHeader::~PacketHeader() {}
PacketHeader::PacketHeader(uint8_t packet_type, uint32_t packet_size) : packet_len{packet_size}, type{packet_type} {}

PacketHeader::PacketHeader(const uint8_t* data, uint8_t size) {
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
			this->packet_len = b;
			step++;
			break;
		case 2:
			this->type = b;
			step++;
			break;
		case 3:
			this->source = b;
			step++;
			break;
		case 4:
			this->destination = b;
			step++;
			break;
		
		}
	}
}

//PacketHeader::~PacketHeader() {}

void PacketHeader::serialize(std::vector<uint8_t>& buf) {
    buf.push_back(start_byte);
	buf.push_back(packet_len);
	buf.push_back(type);
    buf.push_back(source);
	buf.push_back(destination);	
}


void TelemetryPacket::serialize(std::vector<uint8_t>& buf) {
	header.serialize(buf);

	// TODO: Maybe come up with a better way to serialize?
	// Serialize the float variables into bytes
	Packet::serialize_float(x, buf);
	Packet::serialize_float(y, buf);
	Packet::serialize_float(z, buf);
	Packet::serialize_float(vx, buf);
	Packet::serialize_float(vy, buf);
	Packet::serialize_float(vz, buf);
	Packet::serialize_float(ax, buf);
	Packet::serialize_float(ay, buf);
	Packet::serialize_float(az, buf);

	for (int i = sizeof(uint32_t) - 1; i>=0; i--) {
		buf.push_back(system_time >> 8*i);
	}
	buf.push_back(lora_rssi);
	// TODO: Add end of packet byte??
}

void CommandPacket::serialize(std::vector<uint8_t>& buf) {
	header.serialize(buf);
	buf.push_back(command);
}

void DetailedAllPacket::serialize(std::vector<uint8_t>& buf) {
	header.serialize(buf);

	// TODO: Serialize floats into bytes
}

DetailedAllPacket::DetailedAllPacket(const uint8_t* data, const uint8_t size) {
	// TODO: Implement deserializer
}

CommandPacket::CommandPacket(const uint8_t* data, const uint8_t size) {
	header = PacketHeader(data, size); // Deserialize header

	command = data[8]; // First 8 bytes is header
}

TelemetryPacket::TelemetryPacket(const uint8_t* data, const uint8_t size) {
	header = PacketHeader(data, size); // Deserialize the header

	for (int i = 8; i < header.packet_len + 8; i++) { // First 8 bytes is header, start with 9th byte
		switch (i)
		{
		case 8:
			Packet::deserialize_float(x, data + i);
			break;
		case 9:
			Packet::deserialize_float(y, data + i);
			break;
		case 10:
			Packet::deserialize_float(z, data + i);
			break;
		case 11:
			Packet::deserialize_float(ax, data + i);
			break;
		case 12:
			Packet::deserialize_float(ay, data + i);
			break;
		case 13:
			Packet::deserialize_float(az, data + i);
			break;
		case 14:
			Packet::deserialize_float(vx, data + i);
			break;
		case 15:
			Packet::deserialize_float(vy, data + i);
			break;
		case 16:
			Packet::deserialize_float(vz, data + i);
			break;
		case 17:
			for (int j = 0; j < sizeof(uint32_t); j++) {
				system_time |= uint32_t(data[i]) << 8*(sizeof(uint32_t) - j - 1); 
			}
			i += sizeof(uint32_t);
		case 17 + sizeof(uint32_t):
			lora_rssi = data[i];
			break;
		default:
			break;
		}
	}
}

void Packet::serialize_float(const float f, std::vector<uint8_t> &buf) {
	uint8_t data[sizeof(float)];
	memcpy(data, &f, sizeof f);

	for (int i = 0; i < sizeof(float); i++) {
		buf.push_back(data[i]);
	}
}

void Packet::serialize_floats(const float* nums, int num_floats, std::vector<uint8_t> &buf) {
	for (int i = 0; i < num_floats; i++) {
		Packet::serialize_float(nums[i], buf);
	}
}

void Packet::deserialize_float(float &f, const uint8_t* bytes) {
	memcpy(&f, bytes, sizeof f);
}