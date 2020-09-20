/* 
Code used to process states, and the transitions between them, contains parent class for states

Written by the Electronics team, Imperial College London Rocketry
*/

#include "stateMachine.h"






stateMachine::stateMachine() : 
    estimator(),
    downlink(this)
{ 
}


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
  _currStatePtr->exitstate();
  delete _currStatePtr;
  _currStatePtr = newStatePtr;
  _currStatePtr -> initialise();
}
