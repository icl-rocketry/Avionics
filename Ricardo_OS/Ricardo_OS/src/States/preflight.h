/*
State when rocket is not in flight. Allows resetting and intilization of all sensors and check all snesors for anomalies. 
Allows flash system operation to dump/reformat of onboard and sd card.
*/

#ifndef PREFLIGHT_H
#define PREFLIGHT_H
#include "Arduino.h"
#include "state.h"
#include "stateMachine.h"


class Preflight: public State {
  public:
    void initialise();
    State* update();
};

#endif
