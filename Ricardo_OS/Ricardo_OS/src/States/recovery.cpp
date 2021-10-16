#include "Arduino.h"
#include "recovery.h"

#include "flags.h"
#include "stateMachine.h"

#include "Sound/Melodies/melodyLibrary.h"


Recovery::Recovery(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_RECOVERY)
{};

void Recovery::initialise(){
    State::initialise();
    _sm->tunezhandler.play(MelodyLibrary::zeldatheme,true); // play startup sound

};


State* Recovery::update(){


    return this;
};

void Recovery::exitstate(){
    State::exitstate();
    _sm->tunezhandler.clear(); // stop looping zelda
};