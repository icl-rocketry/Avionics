/*
State when rocket is not in flight. Allows resetting and intilization of all sensors and check all snesors for anomalies. 
Allows flash system operation to dump/reformat of onboard and sd card.
If serial input detected switches to usb mode.
Transtion to countdown state happens through lora command
*/

#ifndef PREFLIGHT_H
#define PREFLIGHT_H

#include "state.h"

#include <rnp_routingtable.h>



class Preflight: public State {
  public:
    Preflight(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
    
};

#endif
