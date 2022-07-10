
#include "flight.h"
#include "recovery.h"

#include "flags.h"
#include "stateMachine.h"


Flight::Flight(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_FLIGHT),
altitudeHistory({0,0,0}),
apogeeDelta(0.05)
{};

void Flight::initialise(){
    State::initialise();
};


State* Flight::update(){

    float Ad = _sm->estimator.getData().acceleration(2);

    if (Ad > 0 && !_sm->systemstatus.flag_triggered(SYSTEM_FLAG::FLIGHTPHASE_BOOST)){
        _sm->systemstatus.new_message(SYSTEM_FLAG::FLIGHTPHASE_BOOST,"Entered Boost Phase");
        _sm->systemstatus.delete_message(SYSTEM_FLAG::FLIGHTPHASE_COAST);
    }else if (Ad < 0 && !_sm->systemstatus.flag_triggered(SYSTEM_FLAG::FLIGHTPHASE_COAST)){
        _sm->systemstatus.new_message(SYSTEM_FLAG::FLIGHTPHASE_COAST,"Entered Coast Phase");
        _sm->systemstatus.delete_message(SYSTEM_FLAG::FLIGHTPHASE_BOOST);
    }
    
    if (apogeeDetect()){
        _sm->systemstatus.delete_message(SYSTEM_FLAG::FLIGHTPHASE_COAST);
        _sm->systemstatus.delete_message(SYSTEM_FLAG::FLIGHTPHASE_BOOST);
        _sm->systemstatus.new_message(SYSTEM_FLAG::FLIGHTPHASE_APOGEE,"Apogee Detected!!");
        State* recovery_ptr = new Recovery(_sm);
        return recovery_ptr;
    }else{
        return this;
    }
};

void Flight::exitstate(){
    State::exitstate();
};

bool Flight::apogeeDetect(){ // 20hz

    if (millis() - prevApogeeDetectTime >= apogeeDelta){
        prevApogeeDetectTime = millis();
        altitudeHistory.at(0) = altitudeHistory.at(1);
        altitudeHistory.at(1) = altitudeHistory.at(2);
        altitudeHistory.at(2) = _sm->sensors.getData().baro.alt;

        if ( (altitudeHistory.at(2) < altitudeHistory.at(1)) && (altitudeHistory.at(1) < altitudeHistory.at(0)) && abs(altitudeHistory.at(2) - altitudeHistory.at(0)) > 2){
            return true;
        }
    }


    return false;
}