/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Arduino.h"

#include "States/state.h"

#include "Sensors/estimator.h"
#include "Comms/downlink.h"
#include "Sensors/sensors.h"
#include "SPI.h"
#include "Wire.h"




class stateMachine {
  public:
    stateMachine();// The constructor
    //functions
    void initialise(State* initStatePtr);
    void update();
    void changeState(State* newStatePtr);
    
    SPIClass vspi;
    TwoWire I2C;

    Sensors sensors;
    Estimator estimator;
    Downlink downlink;
    

  private:
    State* _currStatePtr;
};

#endif
