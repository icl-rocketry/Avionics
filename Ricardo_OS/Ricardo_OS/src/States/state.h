#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
#include "flags.h"

class stateMachine; //forward declaration 


class State {
  //friend class stateMachine;

  public:
    State(stateMachine* sm,SYSTEM_FLAG state_id);
    virtual ~State() = 0; //pure virtual destructor
    virtual void initialise();
    virtual State* update() = 0;//pure virtual function 
    virtual void exitstate();

    

    
  protected:
    stateMachine* _sm; //pointer to statemachine
    const SYSTEM_FLAG _curr_stateID;

    uint64_t time_entered_state;
    uint64_t time_exited_state;
    uint64_t time_duration_state;


};


#endif