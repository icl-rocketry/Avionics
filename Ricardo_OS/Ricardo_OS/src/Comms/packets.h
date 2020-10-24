//header file containing declarations of diffrerent packet used 

#ifndef PACKETS_H
#define PACKETS_H
#include <Arduino.h>

enum class packet_type{
    TELEMETRY = 1,
    COMMAND = 2,
    DEATAILED = 3
}

struct telemetry_packet_t{
    uint8_t packet_type = packet_type::TELEMETRY;
    //telemetry data

    //system_status

    //packet details
    bool response_required;

    uint32_t system_time;
    uint8_t packet_id;
    uint8_t lora_rssi;

};

struct command_packet_t{
    uint8_t packet_type = packet_type::COMMAND;
    uint8_t command;
    
};

struct detailed_all_packet_t{
    uint8_t packet_type = packet_type::DETAILED;
    
}





#endif