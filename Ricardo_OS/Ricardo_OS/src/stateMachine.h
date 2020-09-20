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




class stateMachine {
  public:
    stateMachine();// public constructor
    //functions
    void initialise(State* initStatePtr);
    void update();
    void changeState(State* newStatePtr);


    Estimator estimator;
    Downlink downlink;


  private:
    State* _currStatePtr;
};

#endif
