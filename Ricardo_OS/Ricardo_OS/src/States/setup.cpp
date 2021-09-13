#include "ricardo_pins.h"
#include "global_config.h"
#include "Arduino.h"

#include "setup.h"
#include "stateMachine.h"

#include "flags.h"

#include "preflight.h"
#include "groundstation.h"

#include "Sound/Melodies/melodyLibrary.h"



Setup::Setup(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_SETUP)
{};

void Setup::initialise(){
    State::initialise(); 
    _sm->tunezhandler.play(MelodyLibrary::c_scalez); // play startup sound
};


State* Setup::update(){
    //transtion to preflight state
    State* _preflight_ptr = new Preflight(_sm);
    return _preflight_ptr;
};

void Setup::exitstate(){
    State::exitstate();
};