/* 
Inital State machine framework written by Daniele Bella <3
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"
#include <string>
#include <vector>
#include <functional>

#include "States/state.h"

#include "Storage/systemstatus.h"

#include "Storage/logController.h"
#include "Storage/storageController.h"
#include "Storage/configController.h"


#include "Sensors/estimator.h"
#include "Sensors/sensors.h"

#include "Sound/tunezHandler.h"

#include "rnp_networkmanager.h"
#include "rnp_default_address.h"

#include "SPI.h"
#include "Wire.h"





stateMachine::stateMachine() : 
    vspi(VSPI),
    I2C(0),
    storagecontroller(this),
    logcontroller(&storagecontroller),
    systemstatus(&logcontroller),
    usbserial(Serial,systemstatus,logcontroller),
    networkmanager(static_cast<uint8_t>(DEFAULT_ADDRESS::ROCKET),NODETYPE::HUB,true),
    commandhandler(this),
    sensors(this),
    estimator(this)    
{};


void stateMachine::initialise(State* initStatePtr) {

  

  // call tunez handler setup first so we can provide startup tone and auditory cues asap
  tunezhandler.setup();
  //call setup state before callng individual setups
  changeState(initStatePtr);
  //setup storage and logging so any erros encoutered can be logged
  storagecontroller.setup();
  logcontroller.setup();

  // create config controller object
  ConfigController configcontroller(&storagecontroller,&logcontroller); 
  configcontroller.load(); // load configuration from sd card into ram
  //setup interfaces
  usbserial.setup();
  //setup network manager so communication is running
  // add interfaces
  networkmanager.addInterface(&usbserial);
  //load rt table
  networkmanager.enableAutoRouteGen(false);
  networkmanager.setNoRouteAction(RnpNetworkManager::NOROUTE_ACTION::DUMP,{});
  networkmanager.setLogCb([this](const std::string& message){return logcontroller.log(message);});
  networkmanager.registerService(static_cast<uint8_t>(DEFAULT_SERVICES::COMMAND),commandhandler.getCallback()); // register command handler callback
  //sensors must be setup before estimator
  sensors.setup();
  estimator.setup();
 
  
};

void stateMachine::update() {
  //call udpate on tunez handler

  tunezhandler.update();

  //write logs to file 
  logcontroller.update();

  //request new sensor data
  sensors.update();
  //process updated sensor data
  estimator.update();
  logcontroller.log(estimator.state,sensors.sensors_raw);// log new navigation solution and sensor output
  //check for new packets and process
  networkmanager.update();
  

  
  //call update on state after new information has been processed
  State* newStatePtr = _currStatePtr->update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }
};

void stateMachine::changeState(State* newStatePtr) {
  // Delete old state instance and change to new one
  if (_currStatePtr != NULL){
    //if not null pointer call exitstate method
    _currStatePtr->exitstate();

  };

  delete _currStatePtr;
  
  _currStatePtr = newStatePtr;
  _currStatePtr->initialise();



};

