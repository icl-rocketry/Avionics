/* 
Code used to process states, and the transitions between them

Written by the Electronics team, Imperial College London Rocketry
*/

#include "Arduino.h"
#include "states/state.h"

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

class stateMachine {
  public:
    void initialise(State* initStatePtr);
    void update();
    void changeState(State* newStatePtr);

  private:
    State* _currStatePtr;
};

#endif
