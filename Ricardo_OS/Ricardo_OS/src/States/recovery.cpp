#include "Arduino.h"
#include "recovery.h"

#include "flags.h"


Recovery::Recovery(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_RECOVERY)
{};

void Recovery::initialise(){
    State::initialise();


};


State* Recovery::update(){


    return this;
};

void Recovery::exitstate(){
    State::exitstate();
};