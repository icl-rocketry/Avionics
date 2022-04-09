#include "../RocketComponents/deploymenthandler.h"

#include <vector>
#include <memory>
#include <functional>

#include <rnp_networkmanager.h>
#include <ArduinoJson.h>


#include "../RocketComponents/rocketcomponent.h"
#include "../RocketComponents/rocketcomponenttype.h"

#include "../RocketComponents/networkactuator.h"
#include "../RocketComponents/packets/nrcpackets.h"

void DeploymentHandler::setupIndividual_impl(size_t id,JsonObjectConst deployerconfig)
{
   using namespace ConfigurableDynamicHandlerHelpers;

   auto type = getIfContains<std::string>(deployerconfig,"type");


    if (type == "i2c_act_servo"){
        throw std::runtime_error("i2c servo Not implemented!");
    }else if (type == "i2c_act_pyro"){
        throw std::runtime_error("i2c pyro Not implemented!");
    }else if (type == "net_actuator"){
        auto address = getIfContains<uint8_t>(deployerconfig,"address");
        auto destination_service = getIfContains<uint8_t>(deployerconfig,"destination_service");
        addObject(std::make_unique<NetworkActuator>(id, 
                                                    _logcontroller,
                                                    address,
                                                    _serviceID,
                                                    destination_service, 
                                                    _networkmanager));
        //umm i tried okay
        addNetworkCallback(address,
                           destination_service,
                           [this,id](packetptr_t packetptr)
                                {
                                    dynamic_cast<NetworkActuator*>(getObject(id))->networkCallback(std::move(packetptr));
                                }
                            );
            
    }else{
        throw std::runtime_error("Invalid type!");
    }


    
};

uint8_t DeploymentHandler::flightCheck_impl()
{
    uint8_t components_in_error = 0;
    for (auto &component : *this)
    {
        components_in_error += component->flightCheck(_networkRetryInterval,"DeploymentHandler");
    }
    return components_in_error;
}

