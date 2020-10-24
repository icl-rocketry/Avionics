//header file containing declarations of diffrerent packet used 

#ifndef PACKETS_H
#define PACKETS_H
#include <Arduino.h>

enum class packet:uint8_t{
    TELEMETRY = 1,
    COMMAND = 2,
    DETAILED = 3
};

struct telemetry_packet_t{
    uint8_t packet_type = static_cast<uint8_t>(packet::TELEMETRY);
    //telemetry data

    //system_status

    //packet details
    bool response_required;

    uint32_t system_time;
    uint8_t packet_id;
    uint8_t lora_rssi;

};

struct command_packet_t{
    uint8_t packet_type = static_cast<uint8_t>(packet::COMMAND);
    uint8_t command;
    
};

struct detailed_all_packet_t{
    uint8_t packet_type = static_cast<uint8_t>(packet::DETAILED);
    
};





#endif