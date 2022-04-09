#pragma once

#include <iostream>
#include <unistd.h>
#include <string>

#include <ArduinoJson.h>

#include "rocketcomponenttype.h"

#include "stubs.h"


enum class COMPONENT_STATE:uint8_t{
    NOMINAL = 0,
    ERROR = 1
};

struct RocketComponentState{
    uint8_t state; // should this be a bitmask?
    uint32_t lastNewStateUpdateTime;
    uint32_t lastNewStateRequestTime;
};

class RocketComponent{
    public:
        RocketComponent(uint8_t id,RocketComponentTypes::TYPES componentType,LogController& logcontroller):
        _id(id),
        _componentType(componentType),
        _logcontroller(logcontroller)
        {};

        virtual const RocketComponentState* getState() = 0;
        virtual void updateState() = 0;
        virtual bool flightCheck(uint32_t netRetryInterval,std::string handler);
        virtual ~RocketComponent() = 0;

        uint8_t getId(){return _id;};
        RocketComponentTypes::TYPES getComponentType(){return _componentType;};

    protected:
        const uint8_t _id;
        const RocketComponentTypes::TYPES _componentType;

        LogController& _logcontroller;
            

};

