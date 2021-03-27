#ifndef EVENTHANDLER_H
#define EVENTHANDLER_H

#include <Arduino.h>
#include <vector>
#include "event.h"

class StateMachine;


class EventHandler{
    public:
        EventHandler(StateMachine* sm);
        void setup();
        void update();

        bool register_event(Event new_event);

    private:
        StateMachine* _sm; //pointer to statemachine

        std::vector<Event> event_buffer;

        
};


#endif