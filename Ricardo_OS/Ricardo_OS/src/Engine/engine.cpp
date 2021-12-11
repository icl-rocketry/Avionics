#include "engine.h"

#include "Storage/logController.h"
#include "Storage/systemstatus.h"
#include "Pyros/pyroHandler.h"

#include "string"

Engine:: Engine(uint8_t engineID,LogController& logcontroller,SystemStatus& systemstatus,PyroHandler& pyrohandler):
_engineID(engineID),
_logcontroller(logcontroller),
_systemstatus(systemstatus),
_pyrohandler(pyrohandler)
{}

void Engine::start() 
{
    _logcontroller.log("Engine : " + std::to_string(_engineID) + " started...");

}

void Engine::shutdown() 
{
    _logcontroller.log("Engine : " + std::to_string(_engineID) + " shutdown...");
}



