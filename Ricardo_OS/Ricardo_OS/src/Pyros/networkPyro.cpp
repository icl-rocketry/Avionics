#include "networkPyro.h"
#include "pyro.h"


#include "rnp_networkmanager.h"
#include "Storage/systemstatus.h"

NetworkPyro::NetworkPyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,RnpNetworkManager& networkmanager,uint8_t nodeAddress):
Pyro::Pyro(id,duration,systemstatus,PYROTYPE::NETWORKED),
_networkmanager(networkmanager),
_nodeAddress(nodeAddress)
{};

void NetworkPyro::check_continuity(){};

void NetworkPyro::doStuff(){
    Pyro::doStuff(); // call base implmenetation here to log stuff
    // fire code
};

