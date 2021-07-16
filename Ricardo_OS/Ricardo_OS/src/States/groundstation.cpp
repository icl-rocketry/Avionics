#include <Arduino.h>
#include "groundstation.h"
#include "stateMachine.h"

#include "Comms/nodes.h"
#include "flags.h"

Groundstation::Groundstation(stateMachine* sm) : State(sm){
    _curr_stateID = SYSTEM_FLAG::STATE_GROUNDSTATION;
    sm->networkmanager.changeNodeType(NODES::GROUNDSTATION);
};

void Groundstation::initialise(){
    State::initialise();

    
};

State* Groundstation::update(){
    
    return this;//loopy loop
};

void Groundstation::exitstate(){
    State::exitstate();
};