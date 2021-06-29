#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// #include <Arduino.h>
#include <vector>
#include "event.h"

#include <ArduinoJson.h>

class StateMachine;
class jsondoc;

class EventHandler{
    public:
        EventHandler(StateMachine* sm);
        void setup(JsonArray event_config);// configuration dict - 
        //number of events
        //description of each event
        
        void update();
        int timeTriggered(int arg){return 1;};

    private:
        StateMachine* _sm; //pointer to statemachine


        std::vector<Event> eventList;
        bool register_event(Event new_event);
};


#endif