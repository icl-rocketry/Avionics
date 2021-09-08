#ifndef BTPYRO_H
#define BTPYRO_H
//bluetooth pyro 


#include "pyro.h"
#include "rnp_networkmanager.h"
#include "Storage/systemstatus.h"
#include <string>

class BTPyro:public Pyro{
    public:
        BTPyro(uint8_t id,RnpNetworkManager* networkmanager,uint8_t nodeID,SystemStatus* systemstatus);

        bool check_continuity();

        void doStuff(uint8_t duration);

    private:
        RnpNetworkManager* _networkmanager; // pointer to network manager
        uint8_t _nodeID; // node_id of bt pyro

};

#endif