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

/****
 * TODO:
 * Add deployment lockout using a system wide flag
 * disable deploment when debug is entered
 * add deployment full test option where all deploymet channels are tested
 * add servo specialization as deployers can either by pyro or servo based
 * 
 ****/



class PyroHandler{
    public:
        PyroHandler(RnpNetworkManager& rnpnetman,SystemStatus& systemstatus,LogController& logcontroller);

        void setup(JsonArray pyroConfig); // manages construciton of pyro objects
        void update(); // update pyro status 

        

        Pyro* get(int pyroID);

        static constexpr uint8_t PyroHandlerServiceID = static_cast<uint8_t>(DEFAULT_SERVICES::PYRO);
    
    private:
        RnpNetworkManager& _rnpnetman;
        SystemStatus& _systemstatus;
        LogController& _logcontroller;

        std::vector<std::unique_ptr<Pyro> > _pyroList; // vector containing pointers to all pyro objects

        /**
         * @brief network callback used when registering the pyro serivce
         * 
         * @param packet 
         */
        void networkCallback(std::unique_ptr<RnpPacketSerialized> packet);
};

#endif