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
    friend class USB; //usb class needs to know the expected size of a header
public:
    PacketHeader();
    PacketHeader(uint8_t packet_type, uint32_t packet_size); // Initialise a packet
    PacketHeader(const uint8_t* data); // Deserialization constructor
    ~PacketHeader();

    /*
        Serializes the header for network transport.
    */
    void serialize(std::vector<uint8_t>& buf);

    bool header_size_mismatch;

private:
    //packet header 15 bytes
    static const uint8_t _header_size = 15; // Change this variable to reflect the number of bytes in the header
public:
    //header packet defintion
    uint8_t start_byte = 0xAF; // Marks the begin of `Packet`
    uint8_t header_len = _header_size;//header len
    uint32_t packet_len = 0x00000000; // Size of the packet in bytes maybe this should be 32 bit to match size_t and subsequent sizeof() functionality
    uint32_t system_time = 0x00000000; // system time
    uint8_t type = 0x00; // Type of the packet
    uint8_t source = 0x00; // Source interface ID for the packet
    uint8_t destination = 0x00; // Destination interface ID for the packet
    uint8_t src_interface = 0x00; // Source interface ID
    uint8_t ttl = 10; //time to live - prevents infinte rediretion of packets - currently unused
    
    /*
     ▄         ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄  ▄▄        ▄  ▄▄▄▄▄▄▄▄▄▄▄ 
    ▐░▌       ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌▐░░▌      ▐░▌▐░░░░░░░░░░░▌
    ▐░▌       ▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀▀▀▀█░▌▐░▌░▌     ▐░▌ ▀▀▀▀█░█▀▀▀▀ ▐░▌░▌     ▐░▌▐░█▀▀▀▀▀▀▀▀▀ 
    ▐░▌       ▐░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌▐░▌    ▐░▌     ▐░▌     ▐░▌▐░▌    ▐░▌▐░▌          
    ▐░▌   ▄   ▐░▌▐░█▄▄▄▄▄▄▄█░▌▐░█▄▄▄▄▄▄▄█░▌▐░▌ ▐░▌   ▐░▌     ▐░▌     ▐░▌ ▐░▌   ▐░▌▐░▌ ▄▄▄▄▄▄▄▄ 
    ▐░▌  ▐░▌  ▐░▌▐░░░░░░░░░░░▌▐░░░░░░░░░░░▌▐░▌  ▐░▌  ▐░▌     ▐░▌     ▐░▌  ▐░▌  ▐░▌▐░▌▐░░░░░░░░▌
    ▐░▌ ▐░▌░▌ ▐░▌▐░█▀▀▀▀▀▀▀█░▌▐░█▀▀▀▀█░█▀▀ ▐░▌   ▐░▌ ▐░▌     ▐░▌     ▐░▌   ▐░▌ ▐░▌▐░▌ ▀▀▀▀▀▀█░▌
    ▐░▌▐░▌ ▐░▌▐░▌▐░▌       ▐░▌▐░▌     ▐░▌  ▐░▌    ▐░▌▐░▌     ▐░▌     ▐░▌    ▐░▌▐░▌▐░▌       ▐░▌
    ▐░▌░▌   ▐░▐░▌▐░▌       ▐░▌▐░▌      ▐░▌ ▐░▌     ▐░▐░▌ ▄▄▄▄█░█▄▄▄▄ ▐░▌     ▐░▐░▌▐░█▄▄▄▄▄▄▄█░▌
    ▐░░▌     ▐░░▌▐░▌       ▐░▌▐░▌       ▐░▌▐░▌      ▐░░▌▐░░░░░░░░░░░▌▐░▌      ▐░░▌▐░░░░░░░░░░░▌
    ▀▀       ▀▀  ▀         ▀  ▀         ▀  ▀        ▀▀  ▀▀▀▀▀▀▀▀▀▀▀  ▀        ▀▀  ▀▀▀▀▀▀▀▀▀▀▀ 
                                                                                            */  
    /*
    Add new variables after here so nothing gets messed up...
    */ 



};

class Packet {
public:
    static void serialize_float(const float num, std::vector<uint8_t> &buf);
    static void serialize_floats(const float* nums, int num_floats, std::vector<uint8_t> &buf);
    static void deserialize_float(float &f, const uint8_t* bytes);
    static void serialize_uint32_t(const uint32_t &n, std::vector<uint8_t> &buf);
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
    uint8_t lora_rssi;

    // WARNING!
    // Check if all the variables that need to be sent over are getting serialized
    // Whenever a new variable is added to the class it needs to be added to the serialize method
    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    TelemetryPacket(const uint8_t* data);
    TelemetryPacket();
    ~TelemetryPacket();
};

class CommandPacket{
public:
    PacketHeader header {static_cast<uint8_t>(packet::COMMAND), 2};
    uint8_t command;
    uint8_t arg;

    // WARNING!
    // Check if all the variables that need to be sent over are getting serialized
    // Whenever a new variable is added to the class it needs to be added to the serialize method
    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    CommandPacket(const uint8_t* data);
    CommandPacket();
    ~CommandPacket();
};

class DetailedAllPacket{
public:
    PacketHeader header {static_cast<uint8_t>(packet::DETAILED_ALL), 16*sizeof(float) + 2}; // WARNING: Update the size whenever you add/remove variables that are serialized

    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    float gps_lat,gps_long,gps_speed,gps_alt;
    float baro_alt,baro_temp,baro_press;
    uint8_t batt_volt,batt_percent;

    // WARNING!
    // Check if all the variables that need to be sent over are getting serialized
    // Whenever a new variable is added to the class it needs to be added to the serialize method
    void serialize(std::vector<uint8_t>& buf);
    /*
        Deserialization constructor
    */
    DetailedAllPacket(const uint8_t* data);
    DetailedAllPacket();
    ~DetailedAllPacket();
};

#endif