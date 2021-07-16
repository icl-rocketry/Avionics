#include "Arduino.h"
#include "launch.h"
#include "flight.h"

#include "flags.h"

Launch::Launch(stateMachine* sm) : State(sm){
    _curr_stateID = SYSTEM_FLAG::STATE_LAUNCH;
};

void Launch::initialise(){
    State::initialise();


};

State* Launch::update(){

    return this; //loopy loop
};

void Launch::exitstate(){
    State::exitstate();


};

