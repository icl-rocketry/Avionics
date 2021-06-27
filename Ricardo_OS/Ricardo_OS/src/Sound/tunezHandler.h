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
    
        /**
         * @brief Put Melody on tune queue
         * 
         * @param melody 
         * @param loop 
         */
        void play(melody_base_t *melody,bool loop = false);
        /**
         * @brief Skip current tune
         * 
         */
        void skip();
        /**
         * @brief Clear song queue
         * 
         */
        void clear();
        
        void update();

    private:
        
        std::vector<tune_t> tune_queue; 

        uint64_t prev_time = 0;
        uint16_t note_duration = 0;



};

#endif