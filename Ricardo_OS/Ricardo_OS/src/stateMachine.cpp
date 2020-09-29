/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"






stateMachine::stateMachine() : 
    vspi(VSPI),
    I2C(0),
    sensors(this),
    estimator(this),
    downlink(this)
    
{}


void stateMachine::initialise(State* initStatePtr) {
  changeState(initStatePtr);
}

void stateMachine::update() {
  State* newStatePtr = _currStatePtr -> update();

  if (newStatePtr != _currStatePtr) {
    changeState(newStatePtr);
  }
}

void stateMachine::changeState(State* newStatePtr) {
  // Delete old state instance and change to new one
  if (_currStatePtr != NULL){
    _currStatePtr->exitstate();
  };
  delete _currStatePtr;
  _currStatePtr = newStatePtr;
  _currStatePtr -> initialise();
}
