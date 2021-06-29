#include "eventHandler.h"
#include "stateMachine.h"
#include "event.h"
#include <ArduinoJson.h>
#include <stdexcept>




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

bool EventHandler::register_event(Event new_event){
    eventList.push_back(new_event); // add new event to event buffer
    return true;
};

void EventHandler::update(){
    for (auto event : this->eventList) { event.update(); }
};