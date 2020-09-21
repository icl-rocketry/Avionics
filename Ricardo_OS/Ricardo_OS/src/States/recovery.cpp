#include "Arduino.h"
#include "recovery.h"


Recovery::Recovery(stateMachine* sm) : State(sm){
    curr_stateID = 3; //? random id for now
}

void Recovery::initialise(){
    State::initialise();


};


State* Recovery::update(){


    return this;
};

void Recovery::exitstate(){
    State::exitstate();
};