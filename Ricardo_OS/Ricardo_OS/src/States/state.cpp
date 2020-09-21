#include "state.h"

State::State(stateMachine* sm){
    _sm = sm; // assign statemachine pointer 
};

State::~State(){
  // Compulsory virtual destructor definition, even if it's empty
};

void State::initialise(){
    time_entered_state = millis();
    //log these variables using logging object
};



void State::exitstate(){
    time_duration_state = millis() - time_entered_state;
    //log these variables using logging object
};

