/*
State during flight portion of rocket controlling thrust output and relaying telemetry to ground. Limited commands avaliable
*/


#ifndef FLIGHT_H
#define FLIGHT_H
#include "Arduino.h"
#include "state.h"



class Flight: public State {
  public:
    Flight(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
  private:
    bool apogeeDetect();
};

#endif
