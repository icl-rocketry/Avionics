#ifndef STATE_H
#define STATE_H

#include <Arduino.h>

class stateMachine; //forward declaration 

class State {
  public:
    State(stateMachine* sm);
    virtual void initialise();
    virtual State* update();
    virtual void exitstate();
  protected:
    stateMachine* _sm; //pointer to statemachine
    int _stateID;
};


#endif