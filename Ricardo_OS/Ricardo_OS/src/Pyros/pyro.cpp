#include "pyro.h"
#include "Storage/systemstatus.h"
#include "flags.h"

#include "string"

Pyro::Pyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,PYROTYPE pyrotype):
_id(id),
_fireDuration(duration),
_systemstatus(systemstatus),
_pyrotype(pyrotype),
_continuity(false)
{};

void Pyro::doStuff(){
    _systemstatus.new_message(SYSTEM_FLAG::WARN_PYRO,"Pyro " + std::to_string(_id) + " firing for " + std::to_string(_fireDuration) + " seconds!");
    _systemstatus.delete_message(SYSTEM_FLAG::WARN_PYRO);
}