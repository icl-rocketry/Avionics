#include "Arduino.h"
#include "countdown.h"

Countdown::Countdown(stateMachine* sm) : State(sm){
    curr_stateID = 2; //? random id for now
}

void Countdown::initialise(){
    State::initialise();


};

State* Countdown::update(){
    return this;
};

void Countdown::exitstate(){
    State::exitstate();


};

