//header file containing declarations of diffrerent packet used 

#ifndef PACKETS_H
#define PACKETS_H
#include <Arduino.h>
#include <vector>
#include "Logging/serializableElement.h"

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

private:
    //packet header 15 bytes
    

    static constexpr auto getSerializer()
    {
        auto ret = serializer(
            &PacketHeader::start_byte,
            &PacketHeader::header_len,
            &PacketHeader::packet_len,
            &PacketHeader::uid,
            &PacketHeader::type,
            &PacketHeader::source,
            &PacketHeader::destination,
            &PacketHeader::src_interface,
            &PacketHeader::ttl
        );
        return ret;
    }
    static constexpr size_t header_size(){
            return getSerializer().member_size();
    }
   

public:
    PacketHeader();
    PacketHeader(uint8_t packet_type, uint32_t packet_size); // Initialise a packet
    PacketHeader(const std::vector<uint8_t> &data); // Deserialization constructor
    ~PacketHeader();
    
    /*
        Serializes the header for network transport.
    */
   //two serialization methods for testing

    std::vector<uint8_t> serialize() const;
    void serialize(std::vector<uint8_t>& buf) const;

    bool header_size_mismatch;

    //header packet defintion
    uint8_t start_byte = 0xAF; // Marks the begin of `Packet`
    uint8_t header_len = header_size();//header len
    uint32_t packet_len = 0x00000000; // Size of the packet in bytes maybe this should be 32 bit to match size_t and subsequent sizeof() functionality
    uint32_t uid = 0x00000000; // unique id
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

class TelemetryPacket{
    private:
    //serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = serializer(
                &TelemetryPacket::pn,
                &TelemetryPacket::pe,
                &TelemetryPacket::pd,
                &TelemetryPacket::vn,
                &TelemetryPacket::ve,
                &TelemetryPacket::vd,
                &TelemetryPacket::an,
                &TelemetryPacket::ae,
                &TelemetryPacket::ad,
                &TelemetryPacket::roll,
                &TelemetryPacket::pitch,
                &TelemetryPacket::yaw,
                &TelemetryPacket::lat,
                &TelemetryPacket::lng,
                &TelemetryPacket::alt,
                &TelemetryPacket::sat,
                &TelemetryPacket::ax,
                &TelemetryPacket::ay,
                &TelemetryPacket::az,
                &TelemetryPacket::gx,
                &TelemetryPacket::gy,
                &TelemetryPacket::gz,
                &TelemetryPacket::mx,
                &TelemetryPacket::my,
                &TelemetryPacket::mz,
                &TelemetryPacket::temp,
                &TelemetryPacket::press,
                &TelemetryPacket::batt_voltage,
                &TelemetryPacket::batt_percent,
                &TelemetryPacket::launch_lat,
                &TelemetryPacket::launch_lng,
                &TelemetryPacket::launch_alt,
                &TelemetryPacket::system_status,
                &TelemetryPacket::system_time,
                &TelemetryPacket::rssi,
                &TelemetryPacket::snr
                

               
            );
            return ret;
        }
        static constexpr size_t packet_size(){
            return getSerializer().member_size();
        }

    public:
        //packet header
        PacketHeader header{static_cast<uint8_t>(packet::TELEMETRY), packet_size()};
        //estimator output
        float pn, pe, pd; // position NED (m)
        float vn, ve, vd; // velocity NED (m/s)
        float an, ae, ad; // acceleration NED (m/s^2)
        //orientation
        float roll,pitch,yaw; // orientation degrees
        //gps
        long lat,lng,alt;
        uint8_t sat;
        //imu
        float ax, ay, az; // acceleration (g's)
        float gx, gy, gz; // angular rates (deg/s)
        float mx, my, mz;// magnetometer (uT)
        //barometer
        float temp, press;
        //battery
        uint16_t batt_voltage,batt_percent;
        //launch site
        long launch_lat,launch_lng,launch_alt;
        //system details
        uint32_t system_status;
        uint64_t system_time;
        //packet details
        int16_t rssi; 
        float snr;


    
        void serialize(std::vector<uint8_t>& buf) const;
        /*
            Deserialization constructor
        */
        TelemetryPacket(const std::vector<uint8_t> &data);
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
    CommandPacket(const std::vector<uint8_t> &data);
    CommandPacket();
    ~CommandPacket();
};

class DetailedAllPacket{
private:
//serializer framework
        static constexpr auto getSerializer()
        {
            auto ret = serializer(
                &DetailedAllPacket::ax,
                &DetailedAllPacket::ay,
                &DetailedAllPacket::az,
                &DetailedAllPacket::gx,
                &DetailedAllPacket::gy,
                &DetailedAllPacket::gz,
                &DetailedAllPacket::mx,
                &DetailedAllPacket::my,
                &DetailedAllPacket::mz,
                &DetailedAllPacket::gps_lat,
                &DetailedAllPacket::gps_long,
                &DetailedAllPacket::gps_speed,
                &DetailedAllPacket::gps_alt,
                &DetailedAllPacket::baro_alt,
                &DetailedAllPacket::baro_temp,
                &DetailedAllPacket::baro_press,
                &DetailedAllPacket::batt_volt,
                &DetailedAllPacket::batt_percent
            );
            return ret;
        }
        static constexpr size_t packet_size(){
            return getSerializer().member_size();
        }
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
    void serialize(std::vector<uint8_t>& buf) const;
    /*
        Deserialization constructor
    */
    DetailedAllPacket(const std::vector<uint8_t> &data);
    DetailedAllPacket();
    ~DetailedAllPacket();
};

#endif