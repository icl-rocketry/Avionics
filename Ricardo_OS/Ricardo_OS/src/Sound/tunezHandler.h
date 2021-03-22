//hopefully bringing you sick tunes 

#ifndef TUNEZHANDLER_H
#define TUNEZHANDLER_H

#include <Arduino.h>
#include "driver/ledc.h"
#include "Melodies/melodyClass.h"
#include <array>
#include <vector>

struct tune_t{
    melody_base_t* melody;
    uint16_t index = 0; // index in current melody
    bool loop;
};

class TunezHandler{
    public:
        TunezHandler();
        void setup();
        //templated play function as interface so any melody can be played
        template<class T>
        void play(T *melody,bool loop = false){
            playImplementation(static_cast<melody_base_t*>(melody),loop); //static cast here checks at compile time that the class can be cast correctly
        };

        void update();

    private:
        void playImplementation(melody_base_t *melody,bool loop = false);

        melody_base_t* _current_melody; //pointer to current melody object
        std::vector<tune_t> tune_queue; 

        uint64_t prev_time = 0;

        //ledc_timer_config_t ledc_timer;
        //ledc_channel_config_t ledc_channel;

};

#endif