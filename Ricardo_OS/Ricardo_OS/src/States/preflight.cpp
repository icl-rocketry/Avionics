#include "Arduino.h"
#include "preflight.h"
#include "countdown.h"
#include "stateMachine.h"

Preflight::Preflight(stateMachine* sm) : State(sm){
    curr_stateID = 2; //? random id for now
}

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