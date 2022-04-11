#include "simpleengine.h"

#include <memory>

#include <ArduinoJson.h>

#include "RocketComponents/networkactuator.h"
#include "RocketComponents/configurabledynamichandler.h"

#include "Storage/logController.h"

SimpleEngine::SimpleEngine(uint8_t id,JsonObjectConst engineConfig,addNetworkCallbackF_t addNetworkCallbackF,RnpNetworkManager& networkmanager,uint8_t handlerServiceID,LogController& logcontroller):
Engine(id,engineConfig,addNetworkCallbackF,networkmanager,handlerServiceID,logcontroller)
{
    using namespace ConfigurableDynamicHandlerHelpers;
    auto igniterConf = getIfContains<JsonObjectConst>(engineConfig,"igniter");
    auto igniterType = getIfContains<std::string>(igniterConf,"type");

    _igniterParam = getIfContains<uint8_t>(igniterConf,"param");
    
    
    if (igniterType == "net_actuator"){
        auto igniterAddress = getIfContains<uint8_t>(igniterConf,"address");
        auto igniterDestinationService = getIfContains<uint8_t>(igniterConf,"destination_service");
        _igniter = std::make_unique<NetworkActuator>(0,
                                                     logcontroller,
                                                     igniterAddress,
                                                     _handlerServiceID,
                                                     igniterDestinationService,
                                                     networkmanager);
        addNetworkCallbackF(igniterAddress,
                            igniterDestinationService,
                            [this](packetptr_t packetptr)
                                {
                                    dynamic_cast<NetworkActuator*>(_igniter.get())->networkCallback(std::move(packetptr));
                                }
                            ,
                            true
                            );
        
    }else if (igniterType == "i2c_act_pyro"){
        throw std::runtime_error("Not implemented!");
    }else{
        throw std::runtime_error("Invalid igniter type!");
    }

};
        

void SimpleEngine::updateState(){
    _igniter->updateState();
}

uint8_t SimpleEngine::flightCheck(){
    return _igniter->flightCheck(_networkRetryInterval,"SimpleEngine");
}

void SimpleEngine::ignite(){
    Engine::ignite();
    _igniter->execute(_igniterParam); // fire the pyro let it rip
    _state.ignitionTime = millis();
}

void SimpleEngine::shutdown(){ // the engine cant be shut down
    Engine::shutdown();
    _state.shutdownTime = millis();
}

