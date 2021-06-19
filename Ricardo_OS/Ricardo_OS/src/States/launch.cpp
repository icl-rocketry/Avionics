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
   // State* _flight_ptr = new Launch(_sm);
    return this; //loopy loop
};

void Launch::exitstate(){
    State::exitstate();


};

