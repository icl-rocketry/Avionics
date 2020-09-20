#include "state.h"

State::State(stateMachine* sm){
    _sm = sm; // assign statemachine pointer 
};

void State::initialise(){};

State* State::update(){};

void State::exitstate(){};