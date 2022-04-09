#include "controller.h"

Controller::~Controller(){};

void Controller::update(){
    deltaT = millis() - _last_update;
    if (deltaT > _update_interval){
        _last_update = millis();
        calculate();
    }
}