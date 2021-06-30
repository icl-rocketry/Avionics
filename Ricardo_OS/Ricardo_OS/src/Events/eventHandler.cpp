#include "eventHandler.h"
#include "stateMachine.h"
#include "event.h"
#include <ArduinoJson.h>
#include <stdexcept>
#include <memory>




EventHandler::EventHandler(StateMachine* sm):
_sm(sm)
{};


void EventHandler::setup(JsonArray event_config){
    //for each in the list of cool tings

    if (event_config.isNull()){
        return; // dont do nothing we have nothin to do
    }

    for (JsonObject jsonEvent : event_config) {

        std::string type = jsonEvent["type"]; 
        int interface = jsonEvent["interface"];
        bool fire_mode = jsonEvent["single_fire"];
        JsonObject condition = jsonEvent["condition"];

        if (type == "engine"){
            throw std::invalid_argument("Engines not yet supported in config file");
        }
        

    }
};


void EventHandler::update(){
    for (auto &event : eventList) { event->update(); } // get reference as there is no copy semantics for unique_ptr
};

uint32_t EventHandler::timeTriggered(uint8_t eventID){
    try
    {
        return eventList.at(eventID)->timeTriggered();
    }
    catch (std::out_of_range const&)
    {
        return 0; //this is bad
    }
}