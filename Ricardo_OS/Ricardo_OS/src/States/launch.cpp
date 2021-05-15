#include "Arduino.h"
#include "launch.h"
#include "flight.h"

#include "flags.h"

Launch::Launch(stateMachine* sm) : State(sm){
    _curr_stateID = system_flag::STATE_LAUNCH;
};

void Launch::initialise(){
    State::initialise();


};

State* Launch::update(){
    //transtion to flight
    State* _flight_ptr = new Flight(_sm);
    return _flight_ptr;
};

void Launch::exitstate(){
    State::exitstate();


};

