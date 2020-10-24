#include "Arduino.h"
#include "flight.h"
#include "recovery.h"

#include "flags.h"


Flight::Flight(stateMachine* sm) : State(sm){
    _curr_stateID = system_flag::STATE_FLIGHT;
};

void Flight::initialise(){
    State::initialise();


};


State* Flight::update(){

    //at apogee transtion to recovery state
    return this;
};

void Flight::exitstate(){
    State::exitstate();
};