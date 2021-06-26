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
        
        void play(melody_base_t *melody,bool loop = false);
        void pause(); // pauses current playing melody
        void unpause(); // unpauses melody
        void skip(); // moves to next melody
        void clear(); // clears all queued songs
        



        void update();

    private:
        
        std::vector<tune_t> tune_queue; 

        uint64_t prev_time = 0;
        uint16_t note_duration = 0;

        bool _playing;

        //ledc_timer_config_t ledc_timer;
        //ledc_channel_config_t ledc_channel;

};

#endif