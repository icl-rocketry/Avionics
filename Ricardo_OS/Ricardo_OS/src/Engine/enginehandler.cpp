#include "enginehandler.h"

#include <vector>
#include <unordered_map>
#include <string>
#include <ArduinoJson.h>
#include <memory>
#include <functional>

#include <rnp_networkmanager.h>

#include "engine.h"

#include "Storage/logController.h"

#include "simpleengine.h"
#include "hypnos.h"


void EngineHandler::update(){ // call update on all engines
    for (auto& engine : *this){
        engine->update();
    }
}

uint8_t EngineHandler::flightCheck_impl(){
    uint8_t engines_in_error = 0;
    for (auto& engine: *this){
        engines_in_error += engine->flightCheckEngine();
    }
    return engines_in_error;
}



void EngineHandler::setupIndividual_impl(size_t id, JsonObjectConst engineconfig){
    using namespace ConfigurableDynamicHandlerHelpers;

    auto type = getIfContains<std::string>(engineconfig,"type");

    if (type == "SimpleEngine"){
        addObject(std::make_unique<SimpleEngine>(id,
                                                 engineconfig,
                                                 getaddNetworkCallbackFunction(id),
                                                 _networkmanager,
                                                 _serviceID,
                                                 _logcontroller));
    }
    else if (type == "Hypnos"){
        addObject(std::make_unique<Hypnos>(id,
                                            engineconfig,
                                            getaddNetworkCallbackFunction(id),
                                            _networkmanager,
                                            _serviceID,
                                            _logcontroller));   
    }else{
        throw std::runtime_error("Engine of type:" + type + "not implemented!");
    }


}

addNetworkCallbackF_t EngineHandler::getaddNetworkCallbackFunction(uint8_t engineID)
{
    return [this,engineID](uint8_t source, uint8_t source_service, std::function<void(std::unique_ptr<RnpPacketSerialized>)> callback, bool throwOnError)
    {
        auto wrappedCallback = [this,callback,engineID](std::unique_ptr<RnpPacketSerialized> packet_ptr) {
            // check that engine exists
            //calling getObj will check that the id's match and that object exists.
            //If obj doesnt exists, a runtime exception will be thrown
            getObject(engineID);
            callback(std::move(packet_ptr));
        };
        addNetworkCallback(source, source_service, wrappedCallback, throwOnError);
    };
};