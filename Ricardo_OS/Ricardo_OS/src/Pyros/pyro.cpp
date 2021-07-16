#include "pyro.h"
#include "Comms/networkManager.h"
#include "Storage/systemstatus.h"
#include "flags.h"
#include "Storage/utils.h"

Pyro::Pyro(PYRO_TYPE type,SystemStatus* systemstatus,uint8_t id):
_systemstatus(systemstatus),
_type(type),
_continuity(false),
_id(id)
{};

Pyro::~Pyro(){};

void Pyro::doStuff(uint8_t duration){
    _systemstatus->new_message(SYSTEM_FLAG::WARN_PYRO,"Pyro " + utils::tostring(_id) + " firing for " + utils::tostring(duration) + " seconds!");
    _systemstatus->delete_message(SYSTEM_FLAG::WARN_PYRO);
}