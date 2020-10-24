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

struct telemetry_packet_t{
    uint8_t packet_type = static_cast<uint8_t>(packet::TELEMETRY);
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

};

struct command_packet_t{
    uint8_t packet_type = static_cast<uint8_t>(packet::COMMAND);
    uint8_t command;
    
};

struct detailed_all_packet_t{
    uint8_t packet_type = static_cast<uint8_t>(packet::DETAILED_ALL);
    
};





#endif