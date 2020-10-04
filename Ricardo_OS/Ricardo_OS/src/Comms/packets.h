//header file containing declarations of diffrerent packet used 

#ifndef PACKETS_H
#define PACKETS_H
#include <Arduino.h>

struct tx_packet_t{
    //telemetry data

    //system_status

    //packet details
    bool response_required;

    uint32_t system_time;
    uint8_t packet_id;
    uint8_t rssi;

};

struct rx_packet_t{

    uint32_t command;
    


};





#endif