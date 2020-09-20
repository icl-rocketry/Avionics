#include "Arduino.h"
#include "countdown.h"

Countdown::Countdown(stateMachine* sm) : State(sm){
    _stateID = 2; //? random id for now
}

void Countdown::initialise(){

};

