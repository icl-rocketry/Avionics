#include "eventHandler.h"
#include "stateMachine.h"
#include "event.h"


EventHandler::EventHandler(StateMachine* sm):
_sm(sm)
{};


bool EventHandler::register_event(Event new_event){
    event_buffer.push_back(new_event); // add new event to event buffer
};

void EventHandler::update(){
    
};