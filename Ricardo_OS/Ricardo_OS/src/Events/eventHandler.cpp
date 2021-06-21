#include "eventHandler.h"
#include "stateMachine.h"
#include "event.h"


#define JSON_MEMPOOL 200 // Memory pool size for the static json doc in bytes

class jsondoc {};

EventHandler::EventHandler(StateMachine* sm):
_sm(sm)
{};


void EventHandler::setup(jsondoc config){
    //for each in the list of cool tings
    StaticJsonDocument<200> doc;

};

bool EventHandler::register_event(Event new_event){
    eventList.push_back(new_event); // add new event to event buffer
    return true;
};

void EventHandler::update(){
    for (auto event : this->eventList) { event.update(); }
};