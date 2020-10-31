//header file containing declarations of diffrerent packet used 

#ifndef PACKETS_H
#define PACKETS_H
#include <Arduino.h>

enum class packet:uint8_t{
    TELEMETRY = 1,
    COMMAND = 2,
    DETAILED_ALL = 3,
    DETAILED_ACCEL = 4,
    DETAILED_GYRO = 5,
    DETAILED_MAG = 6,
    DETAILED_GPS = 7,
    DETAILED_BATTERY = 8,
    DETAILED_THROTTLE = 9,
    PRINT_FLASH = 10,
    PRINT_SD = 11,
    RAW_SENSOR_OUTPUT = 12,
    ESTIMATOR_OUTPUT = 13,
    MESSAGE = 14
};


class PacketHeader {
public:
    PacketHeader();
    PacketHeader(uint8_t packet_type); // Initialise with a type
    PacketHeader(uint8_t* data, uint8_t size); // Deserialization constructor
    ~PacketHeader();

    /*
        Serializes the header for network transport.
    */
    void serialize(std::vector<uint8_t>& buf);

public:
    uint8_t start_byte = 0xAF; // Marks the begin of `Packet`
    uint8_t type = 0x00; // Type of the packet
    uint8_t source = 0x00; // Source interface ID for the packet
    uint8_t destination = 0x00; // Destination interface ID for the packet
    uint8_t packet_len = 0x00; // Size of the packet in bytes
    uint8_t checksum = 0x00; // XOR checksum.
};


class TelemetryPacket{
    //packet header
    PacketHeader header {static_cast<uint8_t>(packet::TELEMETRY)};
    //telemetry data
    float x,y,z;
    float vx,vy,vz;
    float ax,ay,az;
    //float y,p,r;

    //system_status

    //packet details
    
    uint32_t system_time;
    uint8_t packet_id;
    uint8_t lora_rssi;

    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    TelemetryPacket(uint8_t* data, uint8_t size);
};

class CommandPacket{
    PacketHeader header {static_cast<uint8_t>(packet::COMMAND)};
    uint8_t command;

    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    CommandPacket(uint8_t* data, uint8_t size);
};

class DetailedAllPacket{
    PacketHeader header {static_cast<uint8_t>(packet::DETAILED_ALL)};

    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    DetailedAllPacket(uint8_t* data, uint8_t size);
};

#endif