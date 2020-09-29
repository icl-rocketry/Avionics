#include "buzzer.h"

Buzzer::Buzzer(){};

void Buzzer::setup(){};

void Buzzer::play_melody(uint8_t melody){
    previous_time = millis();
    _melody = melody;   
};

void Buzzer::update(){
    switch (_melody){
        case 0:
            break;
    };
};