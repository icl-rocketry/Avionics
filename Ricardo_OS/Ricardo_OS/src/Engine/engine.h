#pragma once



#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "Pyros/pyroHandler.h"

// base class for engines

enum class ENGINESTATE:uint16_t{
    STARTED = (1<<0),
    SHUTDOWN = (1<<1)

};
struct EngineInfo{
    uint16_t EngineState;
    uint32_t ignitiionTime;
    uint32_t shutdownTime;
};

class Engine{
    public:
        Engine(uint8_t engineID,LogController& logcontroller,SystemStatus& systemstatus,PyroHandler& pyrohandler);

        virtual void start();
        virtual void shutdown();

        virtual ~Engine(){}; // virtual empty destructor 
       
        virtual const EngineInfo* getInfo() = 0;

    protected:
        uint8_t _engineID;
        LogController& _logcontroller;
        SystemStatus& _systemstatus;
        PyroHandler& _pyrohandler;
        // RnpNetworkManager& _networkmanager;
        

        
        
};



