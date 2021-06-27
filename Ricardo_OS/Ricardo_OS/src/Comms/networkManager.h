//manages all communication interfaces with ground
#ifndef NETWORK_MANAGER_H
#define NETWORK_MANAGER_H
#include <Arduino.h>

#include "packets.h"
#include "routingTable.h"


#include "commandHandler.h"

#include "interfaces/interfaces.h"
#include "interfaces/radio.h"
#include "interfaces/usb.h"
//#include "stateMachine.h"

#include "States/preflight.h"
#include "States/groundstation.h"

#include <memory>
#include <vector>




class stateMachine;//forward declaration to prevent circular dependancy


class NetworkManager{

    // allow command handler to get radio rssi and snr
    friend class CommandHandler;

    public:
        NetworkManager(stateMachine* sm);
        
        void setup();
        void update();
        
        void send_to_node(NODES destination,std::vector<uint8_t> &data);
        void send_packet(INTERFACE iface,std::vector<uint8_t> &data);

        uint8_t getNodeType();
        void changeNodeType(NODES node){_nodeType = node;};
        

    protected:
        USB usbserial; //usb serial object
        Radio radio; // lora radio object


    private:
        stateMachine* _sm; //pointer to state machine

        std::vector<std::unique_ptr<std::vector<uint8_t>>> _packet_buffer; //packet buffer containing all network packets received
        //std::vector<std::unique_ptr<std::vector<uint8_t>>> _local_packet_buffer; //packet buffer containing packets meant for this node
     
        RoutingTable routingtable; // routing table for networking -> maybe move to protected so an be acsesed eaiser??
         
        //objects to process commands
        CommandHandler commandhandler;

       
        NODES _nodeType;
        
        void process_packets();

};





#endif