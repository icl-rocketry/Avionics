#include "state.h"
#include "stateMachine.h"

State::State(stateMachine* sm){
    _sm = sm; // assign statemachine pointer 
};

State::~State(){
  // Compulsory virtual destructor definition, even if it's empty
};

void State::initialise(){
    time_entered_state = millis();
    //_sm->systemstatus.new_message(_curr_stateID,'entered at ' + static_cast<String>(time_entered_state));
    _sm->systemstatus.new_message(_curr_stateID,String('entry'));
    //log these variables using logging object
};



void State::exitstate(){
    time_duration_state = millis() - time_entered_state;
     _sm->systemstatus.delete_message(_curr_stateID);
    //log these variables using logging object
};

