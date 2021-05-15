/*
lauches rocket
*/


#ifndef LAUNCH_H
#define LAUNCH_H
#include "Arduino.h"
#include "state.h"


class Launch: public State {
  public:
    Launch(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
};

#endif
