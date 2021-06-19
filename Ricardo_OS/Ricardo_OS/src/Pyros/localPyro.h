#ifndef LOCALPYRO_H
#define LOCALPYRO_H
#include "pyro.h"



class LocalPyro:public Pyro{
    public:
        LocalPyro(uint8_t continuity_pin,uint8_t fire_pin);

        bool check_continuity();

        bool doStuff(uint8_t duration);

    private:
        uint8_t _continuity_pin;
        uint8_t _fire_pin;
};

#endif