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
    PacketHeader(uint8_t packet_type, uint32_t packet_size); // Initialise a packet
    PacketHeader(const uint8_t* data, const uint8_t size); // Deserialization constructor
    ~PacketHeader();

    /*
        Serializes the header for network transport.
    */
    void serialize(std::vector<uint8_t>& buf);

public:
    //packet header 8 bytes
    static const uint8_t header_size = 8; // Change this variable to reflect the number of bytes in the header
    
    uint8_t start_byte = 0xAF; // Marks the begin of `Packet`
    uint32_t packet_len = 0x00000000; // Size of the packet in bytes maybe this should be 32 bit to match size_t and subsequent sizeof() functionality
    uint8_t type = 0x00; // Type of the packet
    uint8_t source = 0x00; // Source interface ID for the packet
    uint8_t destination = 0x00; // Destination interface ID for the packet    
};

class Packet {
public:
    static void serialize_float(const float num, std::vector<uint8_t> &buf);
    static void serialize_floats(const float* nums, int num_floats, std::vector<uint8_t> &buf);
    static void deserialize_float(float &f, const uint8_t* bytes);
};

class TelemetryPacket{
public:
    //packet header
    PacketHeader header {static_cast<uint8_t>(packet::TELEMETRY), 9*sizeof(float) + 5};
    //telemetry data
    float x,y,z;
    float vx,vy,vz;
    float ax,ay,az;
    //float y,p,r;

    //system_status

    //packet details
    
    uint32_t system_time;
    uint8_t lora_rssi;

    // WARNING!
    // Check if all the variables that need to be sent over are getting serialized
    // Whenever a new variable is added here it needs to be added to the serialize method
    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    TelemetryPacket(const uint8_t* data, const uint8_t size);
    TelemetryPacket();
    ~TelemetryPacket();
};

class CommandPacket{
    PacketHeader header {static_cast<uint8_t>(packet::COMMAND), 1};
    uint8_t command;

    // WARNING!
    // Check if all the variables that need to be sent over are getting serialized
    // Whenever a new variable is added here it needs to be added to the serialize method
    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    CommandPacket(const uint8_t* data, const uint8_t size);
    CommandPacket();
    ~CommandPacket();
};

class DetailedAllPacket{
    PacketHeader header {static_cast<uint8_t>(packet::DETAILED_ALL), 0};

    // WARNING!
    // Check if all the variables that need to be sent over are getting serialized
    // Whenever a new variable is added here it needs to be added to the serialize method
    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    DetailedAllPacket(const uint8_t* data, const uint8_t size);
    DetailedAllPacket();
    ~DetailedAllPacket();
};

#endif