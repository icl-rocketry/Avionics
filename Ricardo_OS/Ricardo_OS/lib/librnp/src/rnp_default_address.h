#pragma once

#include <stdint.h>

enum class DEFAULT_ADDRESS:uint8_t{
    NOADDRESS = 0, //only route is debug on this address -> can ping this to get the current address
    DEBUG = 1,
    ROCKET = 2,
    GROUNDSTATION = 3,
    DESKTOP = 4
};