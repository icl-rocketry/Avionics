#pragma once

#include <Arduino.h>
#include <ArduinoJson.h>
#include "rnp_networkmanager.h"

#include "stateMachine.h"

#include "Storage/logController.h"
#include "Storage/systemstatus.h"

// Abstract interface to engines

enum class ENGINESTATE:uint8_t{
    SHUTDOWN,
    IGNITION,
    RUNNING
};
struct EngineInfo{
    ENGINESTATE EngineState;
};

class Engine{
    public:
        Engine(LogController& logcontroller,SystemStatus& systemstatus,RnpNetworkManager& networkmanager); // logcontroller systemstatus networkmanager

        void update(); //????  

        virtual void start();
       
        virtual void stop();
        virtual EngineInfo* getInfo();

    private:

        LogController& _logcontroller;
        SystemStatus& _systemstatus;
        

        
        
};



