#include "simpleEngine.h"

#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "Pyros/pyroHandler.h"

SimpleEngine::SimpleEngine(uint8_t engineID,LogController& logcontroller,SystemStatus& systemstatus,PyroHandler& pyrohandler,uint8_t pyroID):
Engine(engineID,logcontroller,systemstatus,pyrohandler),
_pyroID(pyroID) 
{};


void SimpleEngine::start(){
    Engine::start();
    engineinfo.ignitiionTime = millis();
    _pyrohandler.get(_pyroID)->doStuff(); // fire pyro
    
};

void SimpleEngine::shutdown(){
    Engine::shutdown();
    engineinfo.shutdownTime = millis();
};

const EngineInfo* SimpleEngine::getInfo(){
    if (!_pyrohandler.get(_pyroID)->getContinuity()){
        engineinfo.EngineState |= static_cast<uint16_t>(SIMPLEENGINESTATE::PYROERROR); //no continuity
    }else{
        //we have continuity
        engineinfo.EngineState &= ~static_cast<uint16_t>(SIMPLEENGINESTATE::PYROERROR);
    }
    return &engineinfo;

}