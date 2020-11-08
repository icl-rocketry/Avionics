//manages all communication interfaces with ground
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <Arduino.h>

#include "packets.h"
#include "routingTable.h"

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
        
        void receive_command(Interface iface, uint32_t command);

        
            


    private:
        stateMachine* _sm; //pointer to state machine


        std::vector<uint8_t*> _global_packet_buffer; //packet buffer containing all network packets received

        //std::vector<uint8_t*> _local_packet_buffer; //packet buffer containing packets meant for this node
        

        USB usbserial; //usb serial object
        Radio radio; // lora radio object
        
        //objects to process commands
        CommandBuffer commandbuffer;
        CommandHandler commandhandler;
        
        void NetworkManager::update_buffer(Iface* iface,std::vector<uint8_t*>* buf);
        void NetworkManager::process_global_packets(std::vector<uint8_t*>* global_buf)
        




};





#endif