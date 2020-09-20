/*
State which deploys chutes after rocket has reached apogee.
*/



#ifndef RECOVERY_H
#define RECOVERY_H
#include "Arduino.h"
#include "state.h"




class Recovery: public State {
  public:
    Recovery(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
};

#endif
