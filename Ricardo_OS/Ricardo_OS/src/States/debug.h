
#pragma once

#include <Arduino.h>
#include "state.h"

class Debug: public State {
  public:
    Debug(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
};

