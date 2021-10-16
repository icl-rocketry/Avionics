/*
State during flight portion of rocket controlling thrust output and relaying telemetry to ground. Limited commands avaliable
*/


#ifndef FLIGHT_H
#define FLIGHT_H

#include "state.h"

#include <array>



class Flight: public State {
  public:
    Flight(stateMachine* sm);
    void initialise();
    State* update();
    void exitstate();
  private:
    std::array<float,3> altitudeHistory;
    bool apogeeDetect();
    uint32_t prevApogeeDetectTime;
    const uint16_t apogeeDelta;
};

#endif
