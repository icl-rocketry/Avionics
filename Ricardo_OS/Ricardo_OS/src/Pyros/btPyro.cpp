#include "btPyro.h"
#include "Comms/networkManager.h"

BTPyro::BTPyro(NetworkManager* networkmanager):
Pyro::Pyro(PYRO_TYPE::REMOTE_BT),
_networkmanager(networkmanager)
{};

bool BTPyro::check_continuity(){return true;};

bool BTPyro::doStuff(uint8_t duration){return true;};