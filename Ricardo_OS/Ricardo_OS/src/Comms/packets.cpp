//packet.cpp
#include "packets.h"

PacketHeader::PacketHeader() {}
PacketHeader::~PacketHeader() {}
PacketHeader::PacketHeader(uint8_t packet_type, uint32_t packet_size) : packet_len{packet_size}, type{packet_type} {}

PacketHeader::PacketHeader(const uint8_t* data, const uint8_t size) {
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
			for (int j = sizeof(packet_len) - 1; j >= 0 ; j--) {
				this->packet_len |= data[i+(sizeof(packet_len) - j + 1)] << j*8;
			}
			i+=sizeof(packet_len); // We've read sizeof(packet_len) bytes
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

void PacketHeader::serialize(std::vector<uint8_t>& buf) {
    buf.push_back(start_byte);
	for (int i = sizeof(packet_len) - 1; i >= 0; --i) {
		buf.push_back(packet_len >> i*8);
	}
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
	Packet::serialize_float(ax, buf);
	Packet::serialize_float(ay, buf);
	Packet::serialize_float(az, buf);
	Packet::serialize_float(vx, buf);
	Packet::serialize_float(vy, buf);
	Packet::serialize_float(vz, buf);

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
	header = PacketHeader(data, size); // Deserialize header
	
	// TODO: Deserialize packet
}

CommandPacket::CommandPacket(const uint8_t* data, const uint8_t size) {
	header = PacketHeader(data, size); // Deserialize header

	command = data[header.header_size]; // Get the first byte which is not the header
}

TelemetryPacket::TelemetryPacket(const uint8_t* data, const uint8_t size) {
	header = PacketHeader(data, size); // Deserialize the header

	for (int i = 0; i < header.packet_len; i++) { // First 8 bytes is header, start with 9th byte
		switch (i)
		{
		case 0:
			Packet::deserialize_float(x, data + header.header_size + i*sizeof(float));
			break;
		case 1:
			Packet::deserialize_float(y, data + header.header_size + i*sizeof(float));
			break;
		case 2:
			Packet::deserialize_float(z, data + header.header_size + i*sizeof(float));
			break;
		case 3:
			Packet::deserialize_float(ax, data + header.header_size + i*sizeof(float));
			break;
		case 4:
			Packet::deserialize_float(ay, data + header.header_size + i*sizeof(float));
			break;
		case 5:
			Packet::deserialize_float(az, data + header.header_size + i*sizeof(float));
			break;
		case 6:
			Packet::deserialize_float(vx, data + header.header_size + i*sizeof(float));
			break;
		case 7:
			Packet::deserialize_float(vy, data + header.header_size + i*sizeof(float));
			break;
		case 8:
			Packet::deserialize_float(vz, data + header.header_size + i*sizeof(float));
			break;
		case 9:
			for (int j = 0; j < sizeof(uint32_t); j++) {
				system_time |= uint32_t(data[i]) << 8*(sizeof(uint32_t) - j - 1); 
			}
			i += sizeof(uint32_t);
		case 10 + sizeof(uint32_t):
			lora_rssi = data[i];
			break;
		default:
			break;
		}
	}
}


TelemetryPacket::TelemetryPacket() {}
TelemetryPacket::~TelemetryPacket() {}
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
	memcpy(&f, bytes, sizeof(float));
}