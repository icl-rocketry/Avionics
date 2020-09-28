#include "Arduino.h"
#include "setup.h"

Setup::Setup(stateMachine* sm) : State(sm){
    curr_stateID = 1; //? random id for now
}

void Setup::initialise(){
    State::initialise();


};


State* Setup::update(){


};

void Setup::exitstate(){
    State::exitstate();
};