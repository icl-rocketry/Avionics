#ifndef LOCALPYRO_H
#define LOCALPYRO_H
#include "pyro.h"
#include "Storage/systemstatus.h"


class LocalPyro:public Pyro{
    public:
        LocalPyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,uint8_t continuity_pin,uint8_t fire_pin);

        void check_continuity() override;
        void doStuff() override;

    private:
        const uint8_t _continuity_pin;
        const uint8_t _fire_pin;
};

#endif