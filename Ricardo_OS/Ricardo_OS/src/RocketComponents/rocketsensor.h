#pragma once

#include "rocketcomponent.h"

#include <memory>


struct RocketSensorState:public RocketComponentState{
    float sensorValue; // in si units
};
 //stoopid empty class ...
class RocketSensor:public RocketComponent{
    public:
        RocketSensor(uint8_t id,RocketComponentTypes::TYPES componentType,LogController& logcontroller):
        RocketComponent(id,componentType,logcontroller)
        {};

        virtual ~RocketSensor() = 0;

};

