#include "localPyro.h"
#include "Comms/networkManager.h"
#include "Storage/systemstatus.h"

LocalPyro::LocalPyro(uint8_t id,uint8_t continuity_pin,uint8_t fire_pin,SystemStatus* systemstatus):
Pyro::Pyro(PYRO_TYPE::LOCAL,systemstatus,id),
_continuity_pin(continuity_pin),
_fire_pin(fire_pin)
{};

bool LocalPyro::check_continuity(){return true;};

void LocalPyro::doStuff(uint8_t duration){
    Pyro::doStuff(duration); // call base implmenetation to log the fire command
};