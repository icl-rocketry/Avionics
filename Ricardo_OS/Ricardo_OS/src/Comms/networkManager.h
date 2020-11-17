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

#include "States/preflight.h"
#include "States/groundstation.h"

#include <memory>




class stateMachine;//forward declaration to prevent circular dependancy


class NetworkManager{
    //give these particular states rights to change node type

    friend class Preflight;
    friend class Groundstation;

    public:
        NetworkManager(stateMachine* sm);
        
        void setup();
        void update();
        
        void send_to_node(Nodes destination,uint8_t* data,size_t len);
        void send_packet(Interface iface,uint8_t* data, size_t len);

        void add_command(Nodes source_node, uint32_t command);

        int get_node_type();

    protected:
        //variable to tell network manager the current type of node
        Nodes node_type;

    private:
        stateMachine* _sm; //pointer to state machine


        std::vector<std::shared_ptr<uint8_t>> _global_packet_buffer; //packet buffer containing all network packets received
        std::vector<std::shared_ptr<uint8_t>> _local_packet_buffer; //packet buffer containing packets meant for this node
        

        USB usbserial; //usb serial object
        Radio radio; // lora radio object
        
        //objects to process commands
        CommandBuffer commandbuffer;
        CommandHandler commandhandler;
        
        void process_global_packets();
        void process_local_packets();
        




};





#endif