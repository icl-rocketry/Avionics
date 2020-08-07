/*
State which deploys chutes after rocket has reached apogee.
*/

#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

#ifndef RECOVERY_H
#define RECOVERY_H

class Recovery: public State {
  public:
    void initialise();
    State* update();
};

#endif
