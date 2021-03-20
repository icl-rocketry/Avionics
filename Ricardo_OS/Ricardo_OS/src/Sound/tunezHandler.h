//hopefully using sigma delta modulation to bring you sick tunes 

#ifndef TUNEZHANDLER_H
#define TUNEZHANDLER_H

#include <Arduino.h>
#include "driver/ledc.h"
#include "Melodies/melody_struct.h"


class TunezHandler{
    public:
        TunezHandler();
        void setup();
        void play(melody_t melody,uint8_t priorty);
        void update();

    private:

};

#endif