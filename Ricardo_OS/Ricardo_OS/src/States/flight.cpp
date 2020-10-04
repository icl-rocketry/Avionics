#include "Arduino.h"
#include "flight.h"
#include "recovery.h"


Flight::Flight(stateMachine* sm) : State(sm){
    curr_stateID = 3; //? random id for now
}

void Flight::initialise(){
    State::initialise();


};


State* Flight::update(){

    //at apogee transtion to recovery state
    return this;
};

void Flight::exitstate(){
    State::exitstate();
};