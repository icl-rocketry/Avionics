#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <ArduinoJson.h>
#include <memory>
#include <functional>
#include <unordered_map>

#include <rnp_networkmanager.h>

#include "engine.h"
#include "RocketComponents/flightcomponenthandler.h"


#include "Storage/logController.h"

using addNetworkCallbackF_t = std::function<void(uint8_t,uint8_t,std::function<void(std::unique_ptr<RnpPacketSerialized>)>,bool)>;

class EngineHandler : public FlightComponentHandler<Engine,EngineHandler>{
    public:
        EngineHandler(RnpNetworkManager& networkmanager,uint8_t serviceID,LogController& logcontroller):
        FlightComponentHandler(networkmanager,serviceID,logcontroller)
        {};

        void update(); // calls update on all engines

    protected:
        friend class ConfigurableDynamicHandler;
        void setupIndividual_impl(size_t id,JsonObjectConst engineconfig);

        friend class FlightComponentHandler;
        /**
         * @brief performs flight check on all engines returns the number of engines in error
         * 
         * @return uint8_t 
         */
        uint8_t flightCheck_impl();

        /**
         * @brief Function to allow engines to add network callbacks directly to the network callback map.
         * Wraps the provided network callback to check that engine exists at the correct ID.
         * 
         * @return addNetworkCallbackF_t 
         */
        addNetworkCallbackF_t getaddNetworkCallbackFunction(uint8_t engineID);

};