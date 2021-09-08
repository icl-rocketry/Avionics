
#include "preflight.h"
#include "launch.h"
#include "stateMachine.h"


#include "flags.h"

Preflight::Preflight(stateMachine* sm) : State(sm){
    _curr_stateID = SYSTEM_FLAG::STATE_PREFLIGHT;
    //sm->networkmanager.changeNodeType(NODES::ROCKET);
};

void Preflight::initialise(){
    State::initialise();


};


State* Preflight::update(){
    return this;
};

void Preflight::exitstate(){
    State::exitstate();
};