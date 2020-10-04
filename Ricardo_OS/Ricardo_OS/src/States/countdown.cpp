#include "Arduino.h"
#include "countdown.h"
#include "flight.h"

Countdown::Countdown(stateMachine* sm) : State(sm){
    curr_stateID = 2; //? random id for now
}

void Countdown::initialise(){
    State::initialise();


};

State* Countdown::update(){
    //time based functions for succseful firing of rocket motor -> open valves etc enable taser mode
    //transtion to flight
    State* _flight_ptr = new Flight(_sm);
    return _flight_ptr;
};

void Countdown::exitstate(){
    State::exitstate();


};

