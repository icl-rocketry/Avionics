/*
State when rocket is not in flight. Allows resetting and intilization of all sensors and check all snesors for anomalies. 
Allows flash system operation to dump/reformat of onboard and sd card.
*/
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"

#ifndef PREFLIGHT_H
#define PREFLIGHT_H

class Preflight: public State {
  public:
    void initialise();
    State* update();
};

#endif
