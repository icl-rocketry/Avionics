#pragma once

#include <vector>
#include <memory>

#include <ArduinoJson.h>

#include "event.h"
#include "flightVariables.h"
#include "Storage/logController.h"

#include "stubs.h"

class EventHandler{

    public:

        EventHandler(state_t* state, EngineHandler& enginehandler, DeploymentHandler& deploymenthandler,LogController& logcontroller):
        _flightvariables(state, *this),
        _enginehandler(enginehandler),
        _deploymenthandler(deploymenthandler),
        _logcontroller(logcontroller)
        {};

        void setup(JsonArrayConst event_config);// configuration dict - 
        //number of events
        //description of each event
        
        void update();
        /**.
         * @brief Get the timestamp when an event was triggered.
         * 
         * @param eventID 
         * @return timestamp in milliseconds. Returns 0 if has not been triggered yet
         */
        uint32_t timeTriggered(uint8_t eventID);

        

    private:
    
        FlightVariables _flightvariables;

        EngineHandler& _enginehandler;
        DeploymentHandler& _deploymenthandler;
        
        LogController& _logcontroller;


        action_t configureAction(JsonVariantConst actions);
        condition_t configureCondition(JsonVariantConst condition,uint8_t recursion_level = 0);
        
        static constexpr uint8_t condition_recursion_max_depth = 6;

        #ifdef DEBUG
        std::string _decisiontree = "";
        #endif
        
        std::vector<std::unique_ptr<Event> > _eventList;
};


