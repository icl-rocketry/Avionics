#ifndef PYROHANDLER_H
#define PYROHANDLER_H

#include "pyro.h"

#include "rnp_networkmanager.h"
#include "rnp_packet.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include <vector>
#include <memory>
#include <ArduinoJson.h>
#include <unordered_map>



class PyroHandler{
    public:
        PyroHandler(RnpNetworkManager& rnpnetman,SystemStatus& systemstatus,LogController& logcontroller);

        void setup(JsonArray pyroConfig); // manages construciton of pyro objects
        void update(); // update pyro status 

        

        Pyro* get(int pyroID);

    
    private:
        RnpNetworkManager& _rnpnetman;
        SystemStatus& _systemstatus;
        LogController& _logcontroller;

        std::vector<std::unique_ptr<Pyro> > _pyroList; // vector containing pointers to all pyro objects
        
        std::unordered_map<uint8_t,uint8_t> _pyroNetworkMap; // maping a network address to a pyro id <KEY(node address),VALUE (pyroID)>
        
        /**
         * @brief network callback used when registering the pyro serivce
         * 
         * @param packet 
         */
        void networkCallback(std::unique_ptr<RnpPacketSerialized> packet);
};

#endif