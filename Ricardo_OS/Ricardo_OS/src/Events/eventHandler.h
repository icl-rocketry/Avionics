#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

// #include <Arduino.h>
#include <vector>
#include "event.h"

#include <ArduinoJson.h>
#include <memory>

class StateMachine;


class EventHandler{
    public:
        EventHandler(StateMachine* sm);
        void setup(JsonArray event_config);// configuration dict - 
        //number of events
        //description of each event
        
        void update();
        /**
         * @brief Get the timestamp when an event was triggered.
         * 
         * @param eventID 
         * @return timestamp in milliseconds. Returns 0 if has not been triggered yet
         */
        uint32_t timeTriggered(uint8_t eventID);

    private:
        StateMachine* _sm; //pointer to statemachine


        std::vector<std::unique_ptr<Event> > eventList;
};


#endif