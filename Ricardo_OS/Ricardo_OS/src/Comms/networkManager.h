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

    public:
        NetworkManager(stateMachine* sm);
        
        void setup();
        void update();
        
        void send_to_node(NODES destination,std::vector<uint8_t> &data);
        void send_packet(INTERFACE iface,std::vector<uint8_t> &data);

        uint8_t getNodeType(){return static_cast<uint8_t>(_nodeType);};
        void changeNodeType(NODES node){_nodeType = node;};

        std::vector<double> getInterfaceInfo(INTERFACE interface);


    private:
        stateMachine* _sm; //pointer to state machine

        std::vector<std::unique_ptr<std::vector<uint8_t> > > _packetBuffer; //packet buffer containing all network packets received
        
     
        RoutingTable routingtable;
         
        //objects to process commands
        CommandHandler commandhandler;

        //interfaces
        USB usbserial; //usb serial object
        Radio radio; // lora radio object

       
        NODES _nodeType;
        
        void process_packets();

};





#endif