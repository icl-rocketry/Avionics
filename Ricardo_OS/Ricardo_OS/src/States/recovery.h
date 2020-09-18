/*
State which deploys chutes after rocket has reached apogee.
*/



#ifndef RECOVERY_H
#define RECOVERY_H
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

class Recovery: public State {
  public:
    void initialise();
    State* update();
};

#endif
