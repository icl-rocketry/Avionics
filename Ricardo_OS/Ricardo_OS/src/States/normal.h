/*
State for normal operation of the rocket. Calls immediatley child preflight state. Serial console not avaliable.
*/

#ifndef NORMAL_H
#define NORMAL_H
#include "Arduino.h"
#include "state.h"


class Normal: public State {
  public:
    Normal(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
};

#endif