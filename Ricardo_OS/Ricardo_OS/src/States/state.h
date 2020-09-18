/*
Parent class of the states

Written by Electronics Team, Imperial College London Rocketry
*/


#ifndef STATE_H
#define STATE_H
#include "Arduino.h"


class State {
  public:
    virtual void initialise();
    virtual State* update();
    virtual void exitstate();
};

#endif
