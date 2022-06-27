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


#include "Sensors/estimator.h"

#include "Sensors/sensors.h"

#include "Sound/tunezHandler.h"

#include "Events/eventHandler.h"
#include "Deployment/deploymenthandler.h"
#include "Engine/enginehandler.h"
#include "Controller/controllerhandler.h"

#include "Network/interfaces/usb.h"
#include "Network/interfaces/radio.h"
#include "Network/interfaces/canbus.h"

#include "rnp_networkmanager.h"
#include "rnp_routingtable.h"

#include "Commands/commandHandler.h"

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
    SPIClass hspi;
    TwoWire I2C;


    StorageController storagecontroller;
    LogController logcontroller;

 
    SystemStatus systemstatus;
    
    USB usbserial;
    Radio radio;
    CanBus canbus;
    RnpNetworkManager networkmanager;
    CommandHandler commandhandler;

    Sensors sensors;
    Estimator estimator;

    DeploymentHandler deploymenthandler;
    EngineHandler enginehandler;

    ControllerHandler controllerhandler;
    EventHandler eventhandler;

    
    TunezHandler tunezhandler;
  private:
    State* _currStatePtr;

    static constexpr uint8_t deploymentHandlerServiceID = 4;
    static constexpr uint8_t engineHandlerServiceID = 5;

};

#endif
