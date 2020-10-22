//manages all communication interfaces with ground
#ifndef DOWNLINK_H
#define DOWNLINK_H
#include <Arduino.h>

#include "packets.h"
#include "commandBuffer.h"
#include "commandHandler.h"

#include "interfaces/radio.h"
#include "interfaces/usbserial.h"
//#include "stateMachine.h"




class stateMachine;//forward declaration to prevent circular dependancy


class Downlink{
    public:
        Downlink(stateMachine* sm);
        void setup();
        void update();
        void send_data(uint8_t iface,uint8_t* data, size_t len);
        void receive_command(uint8_t iface, uint32_t command);
    private:
        stateMachine* _sm; //pointer to state machine
        
        tx_packet_t txpacket;
        rx_packet_t rxpacket;

        //objects to process commands
        CommandHandler commandhandler;
        CommandBuffer commandbuffer;

        Radio radio; // lora radio object
        USBserial usbserial;    // usb serial object




};





#endif