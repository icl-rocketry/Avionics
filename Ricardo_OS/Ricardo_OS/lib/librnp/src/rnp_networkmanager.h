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
    COMMAND = 1
};


class RnpNetworkManager {
    public:
        RnpNetworkManager(uint8_t address = 0, NODETYPE nodeType = NODETYPE::LEAF,bool enableLogging = false);
        
        void setup(); // run default setup
        void update(); // run update on all interfaces
        void reset(); //reset routing table to the loaded routing table

        void loadFromJson(); // load from arudino json object
        void saveToNVS();//saves network config to nvs
        void loadFromNVS();//loads network config from nvs

        void sendPacket(RnpPacket &packet); // if u wwant to send raw data use the actual interface
        void sendByRoute(const Route& route, RnpPacket&packet);

        void setAddress(uint8_t address); 
        uint8_t getAddress(){return _currentAddress;};

        void setNodeType(NODETYPE nodeType); // hub or leaf
        NODETYPE getNodeType(){return _nodeType;};

        void addInterface(RnpInterface* iface);
        std::optional<RnpInterface*> getInterface(const uint8_t ifaceID); // checks if a valid interface exists at the proived index
        void removeInterface(const uint8_t ifaceID); // no clue why someone would want to do this
        void removeInterface(RnpInterface* iface); 

        RnpInterfaceInfo* getInterfaceInfo(const uint8_t ifaceID);
        std::string getInterfaceList(){return "";}; //not implemented yet

        void registerService(const uint8_t serviceID, PacketHandlerCb packetHandler); //when deserializing, ensure the lenght in the header and the expected length match
        void unregisterService(const uint8_t serviceID); // remeber to call this on invalidation of function pointer

        void setLogCb(LogCb_t logcb){_logcb = logcb;};

        void enableAutoRouteGen(bool setting){_routeGenEnabled = setting;};

        enum class NOROUTE_ACTION:uint8_t{
            DUMP=0, // Dump the packet
            BROADCAST=1 // broadcast the packet to the specifed interfaces NB careful of packet duplication -> no de-duplication service has been written yet
        };
        void setNoRouteAction(const NOROUTE_ACTION action,const std::vector<uint8_t> ifaces = {});

        void generateDefaultRoutes();
        void updateBaseTable(){_basetable = routingtable;};
        RoutingTable routingtable;

    private:

        void routePackets();
        
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