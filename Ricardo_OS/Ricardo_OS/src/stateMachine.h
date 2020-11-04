/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Arduino.h"

#include "States/state.h"

#include "Logging/systemstatus.h"
#include "Sensors/estimator.h"
#include "Comms/networkManager.h"
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

    SystemStatus systemstatus;
    
    Sensors sensors;
    Estimator estimator;
    NetworkManager networkmanager;
    
    

  private:
    State* _currStatePtr;
};

#endif
