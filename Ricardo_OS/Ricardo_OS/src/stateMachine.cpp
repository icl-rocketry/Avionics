/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"


stateMachine::stateMachine() : 
    vspi(VSPI),
    I2C(0),
    systemstatus(),
    sensors(this),
    estimator(this),
    networkmanager(this),
    storagecontroller(this),
    logcontroller(&storagecontroller),
    configcontroller(&storagecontroller)   
{};


void stateMachine::initialise(State* initStatePtr) {
  //call setup state before callng individual setups
  changeState(initStatePtr);
  //setup classes 
  sensors.setup();
  estimator.setup();
  networkmanager.setup();
  
};

void stateMachine::update() {
  //call update in classes before state update method so state has most recent information
  sensors.update();
  estimator.update();

  networkmanager.update();

  State* newStatePtr = _currStatePtr -> update();

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
  _currStatePtr -> initialise();



};

