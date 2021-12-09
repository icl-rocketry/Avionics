#include "pyroHandler.h"
#include "pyro.h"
#include "networkPyro.h"
#include "localPyro.h"
#include "Packets/pyroPackets.h"

#include "rnp_networkmanager.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include <vector>
#include <memory>
#include <ArduinoJson.h>
#include <unordered_map>
#include <string>


PyroHandler::PyroHandler(RnpNetworkManager& rnpnetman,SystemStatus& systemstatus,LogController& logcontroller):
_rnpnetman(rnpnetman),
_systemstatus(systemstatus),
_logcontroller(logcontroller)
{}

void PyroHandler::setup(JsonArray pyroConfig) 
{
// build them pyro objects into heappppp    
};

void PyroHandler::update(){};


Pyro* PyroHandler::get(uint8_t pyroID) 
{
    if (pyroID >= _pyroList.size()){//out of bounds access
        _logcontroller.log("PyroHandler out of bounds access - pyroID:" + std::tostring(pyroID));
        return nullptr;
    }
    // need to ensure this never crashes lol
    return _pyroList[pyroID].get(); // return a non-owning pointer to callee
};

void Pyrohandler::networkCallback(std::unique_ptr<RnpPacketSerialized> packet_ptr){
    // get pyro id from packet
    uint8_t pyroID = PyroPacket::getPyroID(*packet_ptr);

    Pyro* pyro = get(pyroID); // get pointer to pyro object

    if (pyro == nullptr){
        _logcontroller.log("PyroHandler no pyro found at id supplied");
        return; // check an invalid pyro hasnt been returned
    }

    if (pyro->getType != PYROTYPE::NETWORKED){ // invalid pyro type has been returned -> this should never happen
        _logcontroller.log("PyroHandler bad pyro type");
        return; 
    }

    NetworkPyro* netpyro = dynamic_cast<NetworkPyro*>(pyro);

    if (netpyro == NULL){
        _logcontroller.log("PyroHandler bad dynamic cast");
        return; 
    }
    netpyro->networkCallback(std::move(packet_ptr));
    
}



