#pragma once

#include "rocketcomponent.h"

#include <memory>

#include "Storage/logController.h"

struct RocketActuatorState:public RocketComponentState{
    int32_t currentValue;
};

class RocketActuator: public RocketComponent{
    public:
        RocketActuator(uint8_t id,RocketComponentTypes::TYPES componentType,LogController& logcontroller):
        RocketComponent(id,componentType,logcontroller)
        {};

        virtual void execute(int32_t param) = 0;

        virtual ~RocketActuator() = 0;

};