#include "btPyro.h"
#include "Comms/networkManager.h"

BTPyro::BTPyro(NetworkManager* networkmanager):
Pyro::Pyro(PYRO_TYPE::REMOTE_BT),
_networkmanager(networkmanager)
{};

bool BTPyro::check_continuity(){};

bool BTPyro::Fire(uint8_t duration){};