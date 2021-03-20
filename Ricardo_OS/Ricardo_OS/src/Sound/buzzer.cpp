#include "buzzer.h"


Buzzer::Buzzer(){};

void Buzzer::setup(){};

void Buzzer::play(uint8_t melody,uint8_t priority){
    previous_time = millis();
    _melody = melody;   
};

void Buzzer::update(){
    
};