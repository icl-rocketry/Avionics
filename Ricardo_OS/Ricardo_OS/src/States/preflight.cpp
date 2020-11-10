#include "Arduino.h"
#include "preflight.h"
#include "countdown.h"
#include "stateMachine.h"

#include "Comms/nodes.h"

#include "flags.h"

Preflight::Preflight(stateMachine* sm) : State(sm){
    _curr_stateID = system_flag::STATE_PREFLIGHT;
    sm->networkmanager.node_type = Nodes::ROCKET;
};

void Preflight::initialise(){
    State::initialise();


};


State* Preflight::update(){


    //during preflight if serial command recieved transtion to usbmode
    State* _countdown_ptr = new Countdown(_sm);
    return _countdown_ptr;
};

void Preflight::exitstate(){
    State::exitstate();
};