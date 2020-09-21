#include "Arduino.h"

#include "normal.h"

Normal::Normal(stateMachine* sm) : State(sm){
    curr_stateID = 3; //? random id for now
}

void Normal::initialise(){
    State::initialise();


};


State* Normal::update(){


    return this;
};

void Normal::exitstate(){
    State::exitstate();
};