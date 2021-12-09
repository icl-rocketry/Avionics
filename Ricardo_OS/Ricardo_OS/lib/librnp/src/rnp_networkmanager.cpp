#include <memory>
#include <vector>
#include <optional>
#include <functional>
#include <utility>
#include <iostream>

#include "rnp_networkmanager.h"
#include "rnp_packet.h"
#include "rnp_routingtable.h"
#include "rnp_netman_packets.h"
#include "rnp_default_address.h"

#if defined(ARDUINO)
    #include <Arduino.h>
#endif

RnpNetworkManager::RnpNetworkManager(uint8_t address, NODETYPE nodeType, bool enableLogging):
routingtable(1),
serviceLookup(1), // initalize single element for NETMAN service
_currentAddress(address),
_nodeType(nodeType),
_loggingEnabled(enableLogging)
{
    addInterface(&lo); // add loopback interface
    generateDefaultRoutes();
};

void RnpNetworkManager::setup()
{};

void RnpNetworkManager::update(){
    for (auto iface_ptr : ifaceList){
        if (iface_ptr != nullptr){
            iface_ptr->update(); // call update on all interfaces 
        }
    }
    routePackets();
}

void RnpNetworkManager::reset() 
{
    routingtable = _basetable; // replace the current routing table with the original routing tbale loaded 
    generateDefaultRoutes();
}

void RnpNetworkManager::loadFromJson() 
{
    //extract network manager parametrs from json then pass json to routing table to load
    _basetable.loadJson();
}

void RnpNetworkManager::saveToNVS() 
{
    //serialize to json then save
}

void RnpNetworkManager::loadFromNVS() 
{
    //extract network manager parametrs from json then pass json to routing table to load
    _basetable.loadJson();
};

void RnpNetworkManager::sendPacket(RnpPacket& packet){

    packet.header.hops += 1; // increment hops

    uint8_t destination = packet.header.destination;
    
    std::optional<Route> route = routingtable.getRoute(destination);
    if (!route){
        //bad entry / no entry returned
        switch (_noRouteAction){
            case NOROUTE_ACTION::DUMP:
            {
                return;
            }
            case NOROUTE_ACTION::BROADCAST:
            {
                if (_broadcastList.size() == 0){
                    return;
                }
                for (uint8_t ifaceID : _broadcastList){
                    if ((ifaceID  == packet.header.src_iface) || (ifaceID == static_cast<uint8_t>(DEFAULT_INTERFACES::LOOPBACK))){
                        continue; // dont broadcast packet over the interface it was received
                    }
                
                    sendByRoute({ifaceID,0,{}},packet); //metric is not important and link layer addressing is ignored.
                }
            }
            default:
            {
                return;
            }
        }
    }
    sendByRoute(route.value(),packet);
   
}

void RnpNetworkManager::sendByRoute(const Route& route, RnpPacket& packet) 
{
    //get the corresponding interface index
    uint8_t ifaceID = route.iface;
    if (ifaceID == static_cast<uint8_t>(DEFAULT_INTERFACES::LOOPBACK) && (packet.header.destination != _currentAddress)){
        //bad route
        log("[E] bad route, dest and curr address dont match when sending over loopback");
        return;
    }

    std::optional<RnpInterface*> iface_ptr = getInterface(ifaceID);
    if (!iface_ptr){
        //invalid interface entry returned
        log("[E] invalid/non-existant interface");
        return; 
    }

    packet.header.lladdress = route.address;
    iface_ptr.value()->sendPacket(packet);
};

void RnpNetworkManager::setAddress(uint8_t address) 
{

    auto currentRoute = routingtable.getRoute(_currentAddress);
    if (currentRoute && (currentRoute.value().iface == static_cast<uint8_t>(DEFAULT_INTERFACES::LOOPBACK))){
        routingtable.deleteRoute(_currentAddress); // ensure we dont delete a new route if this is called after a new routing table is assigned
    }
    _currentAddress = address;
    generateDefaultRoutes();
   
};

void RnpNetworkManager::setNodeType(NODETYPE nodeType) 
{
    _nodeType = nodeType;
};

void RnpNetworkManager::addInterface(RnpInterface* iface){
    uint8_t ifaceID = iface->getID();
    //check if id is greater than size of iface list
    if (ifaceID >= ifaceList.size()){
        ifaceList.resize(ifaceID+1);
    }
    if ((ifaceList.at(ifaceID) != nullptr) && (ifaceList.at(ifaceID) != iface)){
        //this means iface has a non unique id which is 
        //print some warning here?
        //or just ignore it
        log("[E] iface non unique id");
    }

    ifaceList.at(ifaceID) = iface;
    iface->setPacketBuffer(&packetBuffer);
};

std::optional<RnpInterface*> RnpNetworkManager::getInterface(const uint8_t ifaceID){
    if (ifaceID >= ifaceList.size()){
        log("[E] iface id out of range");
        return {};
    }
    RnpInterface* iface_ptr = ifaceList.at(ifaceID);
    if (iface_ptr == nullptr){
        return {};
    }
    return {iface_ptr};
};

void RnpNetworkManager::removeInterface(const uint8_t ifaceID){
    if (ifaceID >= ifaceList.size()){
        log("[E] iface id out of range");
        return;
    }
    RnpInterface* iface_ptr = ifaceList[ifaceID];
    iface_ptr->setPacketBuffer(nullptr);
    ifaceList.at(ifaceID) = nullptr; // remove pointer from vector
    if (ifaceID == ifaceList.size()-1){
        ifaceList.erase(ifaceList.end()); // erase last element from ifacelist if the index is the final element in the vector
    }
};

void RnpNetworkManager::removeInterface(RnpInterface* iface){
    uint8_t ifaceID = iface->getID();
    removeInterface(ifaceID);
};

const RnpInterfaceInfo* RnpNetworkManager::getInterfaceInfo(const uint8_t ifaceID){
    auto iface_ptr = getInterface(ifaceID);
    if (!iface_ptr){
        return nullptr;
    }
    return iface_ptr.value()->getInfo();
};

void RnpNetworkManager::registerService(const uint8_t serviceID, PacketHandlerCb packetHandler){
    if (serviceID >= serviceLookup.size()){
        serviceLookup.resize(serviceID+1);
    }
    serviceLookup.at(serviceID) = packetHandler;
}

void RnpNetworkManager::unregisterService(const uint8_t serviceID){
    if (serviceID >= serviceLookup.size()){
        log("[E] service ID out of range");
        return; // out of bounds
    }
    serviceLookup.at(serviceID) = PacketHandlerCb{};
    if (serviceID == serviceLookup.size()-1){
        serviceLookup.erase(serviceLookup.end());
    }
}

void RnpNetworkManager::setNoRouteAction(const NOROUTE_ACTION action,const std::vector<uint8_t> ifaces) 
{
    _noRouteAction = action;
    _broadcastList = ifaces;
}

void RnpNetworkManager::generateDefaultRoutes() 
{
    routingtable.setRoute(_currentAddress,Route{0,1,{}}); // loopback route
    routingtable.setRoute(static_cast<uint8_t>(DEFAULT_ADDRESS::DEBUG),Route{1,1,{}}); // debug port route
};


void RnpNetworkManager::routePackets(){
    if (packetBuffer.empty()){
        return;
    }
    //process packets on buffer one at a time 
    packetptr_t packet_ptr = std::move(packetBuffer.front());
    packetBuffer.pop(); // remove element at front

    if (_routeGenEnabled){//route gen only adds a new route if it didnt previously exist
        std::optional<Route> currentRoute = routingtable.getRoute(packet_ptr->header.source); // check if a route exists to the source node
        if (!currentRoute){
            Route newroute{packet_ptr->header.src_iface,packet_ptr->header.hops,packet_ptr->header.lladdress};
            routingtable.setRoute(packet_ptr->header.source,newroute);
        }
    }

    if ((packet_ptr->header.source == static_cast<uint8_t>(DEFAULT_ADDRESS::DEBUG)) && (packet_ptr->header.destination == static_cast<uint8_t>(DEFAULT_ADDRESS::NOADDRESS))){
        packet_ptr->header.destination = _currentAddress; // process packets addressed to no address on the debug interface as local packets
       
    }

    if (packet_ptr->header.destination != _currentAddress){
        if (_nodeType == NODETYPE::HUB){
            sendPacket(*packet_ptr);
        }
        return;
    }

    if ((packet_ptr->header.source == _currentAddress) && (packet_ptr->header.src_iface != static_cast<uint8_t>(DEFAULT_INTERFACES::LOOPBACK))){ //this can happend if a packet is badly addressed and/or a routing table is bad
        return; 
    }

    uint8_t packetService = packet_ptr->header.destination_service;
    if (packetService == static_cast<uint8_t>(DEFAULT_SERVICES::NETMAN)){ // handle network management packets
        NetManHandler(std::move(packet_ptr));
    }else{
        if (packetService >= serviceLookup.size()){
                //out of bounds access
                return;
            }
            PacketHandlerCb callback = serviceLookup.at(packetService);
            if (!callback){
                //empty function pointer
                return;
            }
            callback(std::move(packet_ptr)); // no copy
    }

};

void RnpNetworkManager::NetManHandler(packetptr_t packet_ptr){
    switch(static_cast<NETMAN_TYPES>(packet_ptr->header.type)){
        case NETMAN_TYPES::PING_REQ:
        {
            PingPacket pong(*packet_ptr);
            std::swap(pong.header.destination,pong.header.source);
            pong.header.type = (uint8_t)NETMAN_TYPES::PING_RES; // change type from request to response
            pong.header.destination_service = pong.header.source_service; //reply ping to orignator service
            pong.header.source_service = (uint8_t)DEFAULT_SERVICES::NETMAN; // update source service
            sendPacket(pong);
            log("ping sent");
            break;
        }
        case NETMAN_TYPES::PING_RES:
        {
            // we receive a ping response
            //currently not handling this
            PingPacket pingres(*packet_ptr);
            log("Ping received with systime of " + std::to_string(pingres.sys_time));
           
            break;
        }
        case NETMAN_TYPES::SET_ADDRESS:
        {
            SetAddressPacket setaddress(*packet_ptr);
            setAddress(setaddress.address);
            log("Node address is now " + std::to_string(setaddress.address));
            break;
        }
        case NETMAN_TYPES::SET_ROUTE:
        {
            SetRoutePacket setroutepacket(*packet_ptr);
            routingtable.setRoute(setroutepacket.destination,setroutepacket.getRoute());
            log("Route for Node " + std::to_string(setroutepacket.destination) + " has been updated");
            break;
        }
        case NETMAN_TYPES::SAVE_CONF:
        {
            saveToNVS();
            break;
        }
        case NETMAN_TYPES::RESET_NETMAN:
        {
            reset();
            break;
        }
        default:
        {
            break;
        }
    }
};


void RnpNetworkManager::log(const std::string& msg){
    if (!_loggingEnabled){
        return;
    }
    if (!_logcb){
        //no callback set use default log
        #if defined(ARDUINO)
            Serial.println(msg.c_str());
        #else
            std::cout<<msg<<"\n";
        #endif

        return;
    }
    _logcb(msg);

} ;