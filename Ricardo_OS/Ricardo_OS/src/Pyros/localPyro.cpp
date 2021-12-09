#include "localPyro.h"
#include "pyro.h"

#include "Storage/systemstatus.h"

LocalPyro::LocalPyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,uint8_t continuity_pin,uint8_t fire_pin):
Pyro::Pyro(id,duration,systemstatus,PYROTYPE::LOCAL),
_continuity_pin(continuity_pin),
_fire_pin(fire_pin)
{};

void LocalPyro::check_continuity(){
    //read continuity pin
};

void LocalPyro::doStuff(){
    Pyro::doStuff(); // call base implmenetation to log the fire command
    //fire the pyro for the duration
    // spawn freertos task to run for the correct duration using isr?

};