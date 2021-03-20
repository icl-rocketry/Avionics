#include "tunezHandler.h"
#include <Arduino.h>
#include "driver/ledc.h"



TunezHandler::TunezHandler(){};

void TunezHandler::setup(){};

void TunezHandler::play(melody_t melody){
    previous_time = millis();
    _melody = melody;   
};

void TunezHandler::update(){
    
};