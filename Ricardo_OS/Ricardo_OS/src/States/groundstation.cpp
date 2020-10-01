#include <Arduino.h>
#include "groundstation.h"
#include "stateMachine.h"

Groundstation::Groundstation(stateMachine* sm) : State(sm){
    curr_stateID = -1;
}

void Groundstation::initialise(){
    State::initialise();
    
};

State* Groundstation::update(){
    
    return this;//loopy loop
};

void Groundstation::exitstate(){
    State::exitstate();
};