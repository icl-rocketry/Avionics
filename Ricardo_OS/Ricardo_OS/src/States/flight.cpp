#include "Arduino.h"
#include "flight.h"
#include "recovery.h"

#include "flags.h"


Flight::Flight(stateMachine* sm) : State(sm){
    _curr_stateID = SYSTEM_FLAG::STATE_FLIGHT;
};

void Flight::initialise(){
    State::initialise();
};


State* Flight::update(){
    
    if (apogeeDetect()){
        State* recovery_ptr = new Recovery(_sm);
        return recovery_ptr;
    }else{
        return this;
    }
};

void Flight::exitstate(){
    State::exitstate();
};

bool Flight::apogeeDetect(){
    return false;
}