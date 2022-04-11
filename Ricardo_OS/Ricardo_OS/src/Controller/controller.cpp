#include "controller.h"
#include <Arduino.h>

Controller::~Controller(){};

void Controller::update(const SensorStructs::state_t& estimator_state){
    deltaT = millis() - _last_update;
    if (deltaT > _update_interval){
        _last_update = millis();
        calculate(estimator_state);
    }
}