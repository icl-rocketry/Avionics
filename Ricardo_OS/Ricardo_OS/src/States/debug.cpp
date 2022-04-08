#include <Arduino.h>
#include "debug.h"

#include "stateMachine.h"

#include "flags.h"

Debug::Debug(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_DEBUG)
{};

void Debug::initialise(){
    State::initialise();
    if (!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){ //indicates first entry
        _sm->networkmanager.registerService(3,_sm->sensors.getHitlCallback()); //register hitl handler callback
    }
};

State* Debug::update(){
    
    return this;
};

void Debug::exitstate(){
    State::exitstate();
    if (!_sm->systemstatus.flag_triggered(SYSTEM_FLAG::DEBUG)){ //indicates exiting out of debug mode completley
        _sm->networkmanager.unregisterService(3); //remove hitl service
    }
};