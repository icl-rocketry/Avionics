//abusing sigma delta modulation to bring you sick tunes 

#ifndef BUZZER_H
#define BUZZER_H

#include <Arduino.h>

class Buzzer{
    public:
        Buzzer();
        void setup();
        void play(uint8_t melody,uint8_t priorty);
        void update();

    private:

        bool playing;
        uint32_t previous_time;
        uint8_t _melody;

};

#endif