#pragma once

#include "engine.h"


#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "Pyros/pyroHandler.h"
//simple engine class with only a single pyro to ignite, no throttling -> represents a normal solid rocket motor

enum class SIMPLEENGINESTATE:uint16_t{ // first 2 indices taken by base class states
    PYROERROR = (1<<2)
};

struct SimpleEngineInfo : public EngineInfo{}; // no extra members needed

class SimpleEngine : public Engine {
    public:

        SimpleEngine(uint8_t engineID,LogController& logcontroller,SystemStatus& systemstatus,PyroHandler& pyrohandler,uint8_t pyroID);

        void start() override;

        void shutdown() override; // does nothing lol

        const EngineInfo* getInfo() override;
        

    private:
        int _pyroID;

        SimpleEngineInfo engineinfo;

};