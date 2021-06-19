/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#ifndef STATEMACHINE_H
#define STATEMACHINE_H

#include "Arduino.h"

#include "States/state.h"

#include "Storage/systemstatus.h"

#include "Storage/logController.h"
#include "Storage/storageController.h"
#include "Storage/configController.h"

#include "Sensors/estimator.h"
#include "Comms/networkManager.h"
#include "Sensors/sensors.h"

#include "Sound/tunezHandler.h"


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


    StorageController storagecontroller;
    LogController logcontroller;
    ConfigController configcontroller;

    SystemStatus systemstatus;
    
    NetworkManager networkmanager;

    Sensors sensors;
    Estimator estimator;
    
    TunezHandler tunezhandler;
  private:
    State* _currStatePtr;

};

#endif
