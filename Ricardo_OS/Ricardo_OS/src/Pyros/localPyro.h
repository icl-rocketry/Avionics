#ifndef LOCALPYRO_H
#define LOCALPYRO_H
#include "pyro.h"
#include "Storage/systemstatus.h"


class LocalPyro:public Pyro{
    public:
        LocalPyro(uint8_t id,uint8_t continuity_pin,uint8_t fire_pin,SystemStatus* systemstatus);

        bool check_continuity();

        void doStuff(uint8_t duration);

    private:
        uint8_t _continuity_pin;
        uint8_t _fire_pin;
};

#endif