#include "engine.h"

#include <ArduinoJson.h>
#include <memory>
#include <rnp_networkmanager.h>

#include "RocketComponents/rocketcomponent.h"


#include "Storage/logController.h"

void Engine::execute(int func){
    switch(func){
        case 1:
        {
            ignite();
            break;
        }
        case 0:
        {
            shutdown();
            break;
        }
        default:
        {
            break;
        }
    }
};

void Engine::ignite(){
    _logcontroller.log("Engine: " + std::to_string(getID()) + " ignition called!");
};

void Engine::shutdown(){
    _logcontroller.log("Engine: " + std::to_string(getID()) + "shutdown called!" );
};

Engine::~Engine(){};