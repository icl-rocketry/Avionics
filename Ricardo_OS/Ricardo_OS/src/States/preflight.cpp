#include "Arduino.h"
#include "preflight.h"
#include "launch.h"
#include "stateMachine.h"

#include "Comms/nodes.h"

#include "flags.h"

Preflight::Preflight(stateMachine* sm) : State(sm){
    _curr_stateID = system_flag::STATE_PREFLIGHT;
    sm->networkmanager.node_type = Nodes::ROCKET;
};

void Preflight::initialise(){
    State::initialise();


};


State* Preflight::update(){

    //Serial.println(_sm->sensors.sensors_raw.gps_lat);
    //Serial.println(_sm->sensors.sensors_raw.gps_long);
    //Serial.println(_sm->sensors.sensors_raw.batt_percent);
    
    return this;
};

void Preflight::exitstate(){
    State::exitstate();
};