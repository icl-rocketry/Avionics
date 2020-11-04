//manages all communication interfaces with ground
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <Arduino.h>

#include "packets.h"
#include "commandBuffer.h"
#include "commandHandler.h"

#include "interfaces/interfaces.h"
#include "interfaces/radio.h"
#include "interfaces/usb.h"
//#include "stateMachine.h"




class stateMachine;//forward declaration to prevent circular dependancy


class NetworkManager{
    public:
        NetworkManager(stateMachine* sm);
        void setup();
        void update();
        void send_data(Interface iface,uint8_t* data, size_t len);
        
        void receive_command(uint8_t iface, uint32_t command);

        
            


    private:
        stateMachine* _sm; //pointer to state machine
        
        std::vector<uint8_t*> _packet_buffer; 

        USB usbserial; //usb serial object
        Radio radio; // lora radio object
        
        
        //objects to process commands
        CommandBuffer commandbuffer;
        CommandHandler commandhandler;
        

        




};





#endif