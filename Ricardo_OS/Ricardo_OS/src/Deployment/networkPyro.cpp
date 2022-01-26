#include "networkPyro.h"
#include "pyro.h"


#include "Commands/commandpacket.h"
#include "Packets/pyroPackets.h"


#include "rnp_networkmanager.h"
#include "Storage/systemstatus.h"

NetworkPyro::NetworkPyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,RnpNetworkManager& networkmanager,uint8_t nodeAddress):
Pyro::Pyro(id,duration,systemstatus,PYROTYPE::NETWORKED),
_networkmanager(networkmanager),
_nodeAddress(nodeAddress)
{};

void NetworkPyro::check_continuity(){

    SimpleCommandPacket check_continuity_command(2,_id); // pyro id in arg, command id = 2 for check continuity 
    check_continuity_command.header.source = _networkmanager.getAddress();
    check_continuity_command.header.source_service = static_cast<uint8_t>(DEFAULT_SERVICES::PYRO);
    check_continuity_command.header.destination = _nodeAddress;
    
    _networkmanager.sendPacket(check_continuity_command);

};

void NetworkPyro::doStuff(){
    Pyro::doStuff(); // call base implmenetation here to log stuff
    // fire code

    // this should be changed to an authenticated packet when written
    SimpleCommandPacket fire_command(3,_id); // pyro id in arg, command id = 2 for check continuity 
    fire_command.header.source = _networkmanager.getAddress();
    fire_command.header.source_service = static_cast<uint8_t>(DEFAULT_SERVICES::PYRO);
    fire_command.header.destination = _nodeAddress;
    
    _networkmanager.sendPacket(fire_command);
}

void NetworkPyro::networkCallback(std::unique_ptr<RnpPacketSerialized> packet_ptr) 
{
    // check packet came from expected source
    if (packet_ptr->header.type == static_cast<uint8_t>(PyroPacket::TYPES::PYROINFOPACKET)){
        if (packet_ptr->header.source != _nodeAddress){
            return;
        }
        PyroInfoPacket pyroinfo(*packet_ptr);
        _continuity = pyroinfo.continuity; // update continuity
    }

};

