//hopefully using sigma delta modulation to bring you sick tunes 

#ifndef TUNEZHANDLER_H
#define TUNEZHANDLER_H

#include <Arduino.h>
#include "driver/ledc.h"
#include "Melodies/melodyClass.h"


class TunezHandler{
    public:
        TunezHandler();
        void setup();
        void play(melody_base_t *melody);
        void update();

    private:

};

#endif