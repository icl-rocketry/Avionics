//manages all communication interfaces with ground
#ifndef DOWNLINK_H
#define DOWNLINK_H
#include "Arduino.h"
//#include "stateMachine.h"


struct tx_packet_t{
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

class stateMachine;//forward declaration to prevent circular dependancy


class Downlink{
    public:
        Downlink(stateMachine* sm);
        void setup();
        void send_data(uint8_t iface,uint8_t* data, size_t len);
    private:
        stateMachine* _sm; //pointer to state machine
        tx_packet_t txpacket;
        recieved_packet_t rxpacket;


};





#endif