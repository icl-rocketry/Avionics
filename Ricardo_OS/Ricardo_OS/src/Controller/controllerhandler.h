#pragma once

#include <vector>
#include <memory>

#include <ArduinoJson.h>

#include "controller.h"
#include "controllable.h"
#include "../Engine/enginehandler.h"

#include "../RocketComponents/configurabledynamichandler.h"

#include "stubs.h"

class ControllerHandler :public ConfigurableDynamicHandler<Controller,ControllerHandler>{
    public:
        ControllerHandler(const state_t& estimatorstate,EngineHandler& enginehandler,LogController& logcontroller):
            ConfigurableDynamicHandler(logcontroller), 
            _estimator_state(estimatorstate), _enginehandler(enginehandler)
        {};

       
        
        void update();
    protected:
        const state_t& _estimator_state;

        EngineHandler& _enginehandler; // currently only supporting engines but can be expanded later to support controllable fins or more!

        Controllable* getControllable(JsonObjectConst config);

        friend class ConfigurableDynamicHandler;
        void setupIndividual_impl(size_t id,JsonObjectConst controllerconfig);
        
    
};