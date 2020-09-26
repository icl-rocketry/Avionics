#include "estimator.h"
#include "stateMachine.h"

Estimator::Estimator(stateMachine* sm){
    _sm = sm;
}

void Estimator::setup(){

};

void Estimator::update(){
    int batt_voltage = _sm->sensors.sensors_raw.batt_volt; //example to get value
};