#ifndef BTPYRO_H
#define BTPYRO_H
//bluetooth pyro 


#include "pyro.h"
#include "Comms/networkManager.h"

class BTPyro:public Pyro{
    public:
        BTPyro(NetworkManager* networkmanager);

        bool check_continuity();

        bool doStuff(uint8_t duration);

    private:
        NetworkManager* _networkmanager; // pointer to network manager

};

#endif