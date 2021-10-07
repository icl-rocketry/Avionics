#include "state.h"
#include "flags.h"
#include "stateMachine.h"

#include <string>

State::State(stateMachine* sm,SYSTEM_FLAG state_id):
_sm(sm),
_curr_stateID(state_id)
{};

State::~State(){
  // Compulsory virtual destructor definition, even if it's empty
};

void State::initialise(){
    time_entered_state = millis();
    _sm->systemstatus.new_message(_curr_stateID,"state entered");
};



void State::exitstate(){
    time_exited_state = millis();
    time_duration_state = time_exited_state - time_entered_state;
     _sm->systemstatus.delete_message(_curr_stateID,"state exited | state duration: " + std::to_string(time_duration_state));

};

