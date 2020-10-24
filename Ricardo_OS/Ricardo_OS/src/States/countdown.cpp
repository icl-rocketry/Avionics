#include "Arduino.h"
#include "countdown.h"
#include "flight.h"

#include "flags.h"

Countdown::Countdown(stateMachine* sm) : State(sm){
    _curr_stateID = system_flag::STATE_COUNTDOWN;
};

void Countdown::initialise(){
    State::initialise();


};

State* Countdown::update(){
    //time based functions for succseful firing of rocket motor -> open valves etc enable taser mode
    //transtion to flight
    State* _flight_ptr = new Flight(_sm);
    return _flight_ptr;
};

void Countdown::exitstate(){
    State::exitstate();


};

