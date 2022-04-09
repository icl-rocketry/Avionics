#pragma once

#include <cstdint>


namespace RocketComponentTypes{
    enum class TYPES: uint16_t{
        I2C_ACT_SERVO = (1 << 0), //Local I2C Servo Actuator 0, 0000
        I2C_ACT_PYRO = (1 << 1),  //Local I2C Pyro Actuator  1, 0001
        I2C_SENS_ADC = (1 << 2), //Local I2C ADC channel 2, 0010
        NET_ACTUATOR = (1 << 3), //3, 0100
        NET_SENSOR = (1 << 4), //4, 1000
        IS_NETWORKED_MASK = (NET_ACTUATOR | NET_SENSOR)
    };

    inline bool isNetworked(RocketComponentTypes::TYPES type) {
        return static_cast<uint16_t>(RocketComponentTypes::TYPES::IS_NETWORKED_MASK) & static_cast<uint16_t>(type);
    }


};
