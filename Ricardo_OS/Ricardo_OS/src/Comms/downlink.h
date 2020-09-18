//manages all communication interfaces with ground
#ifndef DOWNLINK_H
#define DOWNLINK_H
#include "Arduino.h"
#include "interfaces/radio.h"
struct telemetry_packet_t{
    //telemetry data

    //system_status

    //packet details
    uint32_t system_time;
    uint8_t packet_id;
    uint8_t rssi;

};

struct recieved_packet_t{

    uint32_t command;
    
};

class Downlink{
    public:
        void setup();
        void send_data(uint8_t iface,uint32_t* data, size_t len);
    private:



};





#endif