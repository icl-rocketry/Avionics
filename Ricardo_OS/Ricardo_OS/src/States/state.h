#ifndef STATE_H
#define STATE_H

#include <Arduino.h>
//enum containing state id's bit masked
enum class state{
    PREFLIGHT = ( 1 << 0),


};


class stateMachine; //forward declaration 

class State {
  friend class stateMachine;

  public:
    State(stateMachine* sm);
    virtual ~State() = 0; //pure virtual destructor
    virtual void initialise();
    virtual State* update() = 0;//pure virtual function 
    virtual void exitstate();

    
  protected:
    stateMachine* _sm; //pointer to statemachine
    uint8_t curr_stateID;
    uint32_t time_entered_state;
    uint32_t time_duration_state;


};


#endif