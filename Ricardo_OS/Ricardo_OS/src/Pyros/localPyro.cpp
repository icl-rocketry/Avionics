#include "localPyro.h"
#include "Comms/networkManager.h"

LocalPyro::LocalPyro(uint8_t continuity_pin,uint8_t fire_pin):
Pyro::Pyro(PYRO_TYPE::LOCAL),
_continuity_pin(continuity_pin),
_fire_pin(fire_pin)
{};

bool LocalPyro::check_continuity(){return true;};

bool LocalPyro::doStuff(uint8_t duration){return true;};