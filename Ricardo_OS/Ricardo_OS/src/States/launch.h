/*
lauches rocket
*/


#ifndef LAUNCH_H
#define LAUNCH_H

#include "state.h"


class Launch: public State {
  public:
    Launch(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
};

#endif
