#include "simpleEngine.h"

#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "Pyros/pyroHandler.h"

SimpleEngine::SimpleEngine(uint8_t engineID,LogController& logcontroller,SystemStatus& systemstatus,PyroHandler& pyrohandler,uint8_t pyroID):
Engine(engineID,logcontroller,systemstatus,pyrohandler),
_pyroID(pyroID) 
{};


void SimpleEngine::start(){
    _pyrohandler.fire(_pyroID);
    engineinfo.ignitiionTime = millis();
    
};

void SimpleEngine::shutdown(){
    engineinfo.shutdownTime = millis();
};

const EngineInfo* SimpleEngine::getInfo(){
    _pyrohandler.
}