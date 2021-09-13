#pragma once

//TODO
// Provide implementation form loading from json including the nvs loads
// provide implementaiton in routng tbale from loading and serializing to json


#include <memory>
#include <vector>
#include <queue>
#include <string>
#include <array>
#include <functional>


#include "rnp_packet.h"
#include "rnp_header.h"
#include "rnp_routingtable.h"
#include "rnp_interface.h"
#include "loopback.h"

using packetptr_t = std::unique_ptr<RnpPacketSerialized>;
using packetBuffer_t = std::queue<packetptr_t>;
using PacketHandlerCb = std::function<void(packetptr_t)>;
using LogCb_t = std::function<void(const std::string&)>;


enum class NODETYPE : uint8_t{
            LEAF=0, // only accepts packets addressed to this node and dumps any others. 
            HUB=1, // accepts all packets and forwards to the correct interface (promiscous mode)
};

enum class DEFAULT_SERVICES:uint8_t{
    NETMAN = 0,
    COMMAND = 1,
    TELEMETRY = 2
};


class RnpNetworkManager {
    public:
        RnpNetworkManager(uint8_t address = 0, NODETYPE nodeType = NODETYPE::LEAF,bool enableLogging = false);
        
        /**
         * @brief Runs default setup -> currently nothing
         * 
         */
        void setup(); 
        /**
         * @brief Runs update routine on all interfaces in the iflist, also calls routePacket command to process any received packets.
         * 
         */
        void update(); 
        /**
         * @brief Resets the network configuration including routing table to the orignal loaded configuration
         * 
         */
        void reset(); //reset routing table to the loaded routing table

        /**
         * @brief Loads config from a json string
         * 
         */
        void loadFromJson();
        /**
         * @brief saves network configuration to Non Volatile Storage (esp32 specific),
         * configruation is stored as a json string
         * 
         */
        void saveToNVS();//saves network config to nvs
        /**
         * @brief Loads network configruation from Non Volatile Storage
         * 
         */
        void loadFromNVS();//loads network config from nvs

        /**
         * @brief Sends a RnpPacket. All routing information is contained within the header of the packet.
         * 
         * @param packet RnpPacket type, typically a class derived from RnpPacket is passed
         */
        void sendPacket(RnpPacket &packet); // if u wwant to send raw data use the actual interface
        /**
         * @brief send packet over a specified route
         * 
         * @param route 
         * @param packet 
         */
        void sendByRoute(const Route& route, RnpPacket&packet);

        /**
         * @brief Set the Address of the node, this removes the previous loopback route and generates a new loopback route aswell so that the loopback route always exists
         * 
         * @param address 
         */
        void setAddress(uint8_t address); 
        /**
         * @brief Get the Address of the node.
         * 
         * @return uint8_t 
         */
        uint8_t getAddress(){return _currentAddress;};

        /**
         * @brief Set the Node Type
         * 
         * @param nodeType 
         */
        void setNodeType(NODETYPE nodeType); // hub or leaf
        /**
         * @brief Get the Node Type
         * 
         * @return NODETYPE 
         */
        NODETYPE getNodeType(){return _nodeType;};

        /**
         * @brief Add interface to iface list, places the interface at the id speicifed in the interface object
         * 
         * @param iface Pointer to interface object
         */
        void addInterface(RnpInterface* iface);
        /**
         * @brief Get the Interface object from ifacelist
         * 
         * @param ifaceID id of interface
         * @return std::optional<RnpInterface*> 
         */
        std::optional<RnpInterface*> getInterface(const uint8_t ifaceID); 
        /**
         * @brief Remove interface at iface id
         * 
         * @param ifaceID 
         */
        void removeInterface(const uint8_t ifaceID); 
        /**
         * @brief Remove interface by iface pointer 
         * 
         * @param iface 
         */
        void removeInterface(RnpInterface* iface); 

        /**
         * @brief Get Interface info
         * 
         * @param ifaceID 
         * @return RnpInterfaceInfo* 
         */
        const RnpInterfaceInfo* getInterfaceInfo(const uint8_t ifaceID);
        /**
         * @brief Get List of interfaces
         * 
         * @return std::string 
         */
        std::string getInterfaceList(){return "";}; //not implemented yet

        /**
         * @brief Register a packet callback handler for a specifed service id. Thread saftey is up to the implmentation of the callback.
         * check that the expect packet length and recieved packet length match
         * 
         * @param serviceID 
         * @param packetHandler 
         */
        void registerService(const uint8_t serviceID, PacketHandlerCb packetHandler);
        /**
         * @brief remove callback by id, ensure this is called if the callback passed gets destructed otherwise there will be UB
         * 
         * @param serviceID 
         */
        void unregisterService(const uint8_t serviceID); 

        /**
         * @brief Pass a logging function pointer to allow logging of errors from network manager
         * 
         * @param logcb 
         */
        void setLogCb(LogCb_t logcb){_logcb = logcb;};

        /**
         * @brief Enable the learning of routes by analyzing packets comming from previously unkown sources
         * 
         * @param setting 
         */
        void enableAutoRouteGen(bool setting){_routeGenEnabled = setting;};

        
        enum class NOROUTE_ACTION:uint8_t{
            DUMP=0, /*! Dumps the packet !*/
            BROADCAST=1 /*! broadcast the packet to the specifed interfaces NB careful of packet duplication -> no de-duplication service has been written yet !*/
        };
        /**
         * @brief Set action if no route is found in the routing table.
         * 
         * @param action 
         * @param ifaces list of interface ids to broadcast the packet on
         */
        void setNoRouteAction(const NOROUTE_ACTION action,const std::vector<uint8_t> ifaces = {});

        /**
         * @brief Generates the loopback route aswell as the debug port route
         * 
         */
        void generateDefaultRoutes();
        /**
         * @brief updates the _basetable to the current routing table
         * 
         */
        void updateBaseTable(){_basetable = routingtable;};
        RoutingTable routingtable;
        
    private:

        /**
         * @brief Processes any recieved packets 
         * 
         */
        void routePackets();
        
        /**
         * @brief Internal network management packets handler
         * 
         * @param packet_ptr 
         */
        void NetManHandler(packetptr_t packet_ptr); //internal network packet handler

        packetBuffer_t packetBuffer;

        std::vector<PacketHandlerCb> serviceLookup;

        Loopback lo; // loopback is by default owned by the network manager
        std::vector<RnpInterface*> ifaceList;

        uint8_t _currentAddress;
        NODETYPE _nodeType;

        NOROUTE_ACTION _noRouteAction;
        std::vector<uint8_t> _broadcastList;

        bool _routeGenEnabled;

        RoutingTable _basetable; //copy of the intial routing table read in from config
       


        const bool _loggingEnabled;
        LogCb_t _logcb; // callback to logging function
        void log(const std::string& msg);




};