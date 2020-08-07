/*
State for normal operation of the rocket. Calls immediatley child preflight state. Serial console not avaliable.
*/
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

#ifndef NORMAL_H
#define NORMAL_H

class Normal: public State {
  public:
    void initialise();
    State* update();
};

#endif