#pragma once

#include <Arduino.h>

enum class SYSTEM_FLAG:uint32_t{
    //state flags
    STATE_DEBUG = (1 << 0), 
    STATE_PREFLIGHT = (1 << 1),
    STATE_LAUNCH = (1 << 2),
    STATE_FLIGHT = (1 << 3),
    STATE_RECOVERY = (1 << 4),
    STATE_SETUP = (1 << 5),
    STATE_GROUNDSTATION = (1 << 6),
    //flags
    DEBUG = (1 << 7),
    //critical messages 
    ERROR_SPI = (1 << 8),
    ERROR_I2C = (1 << 9),
    ERROR_SERIAL = (1 << 10),
    ERROR_LORA = (1 << 11),
    ERROR_BARO = (1 << 12),
    ERROR_BATT = (1 << 13),
    ERROR_GPS = (1 << 14),
    ERROR_IMU = (1 << 15),
    ERROR_HACCEL = (1 << 16),
    ERROR_MAG = (1 << 17),
    ERROR_ESTIMATOR = (1 << 18),
    ERROR_SD = (1 << 19),
    ERROR_FLASH = (1 << 20),
    ERROR_CAN = (1 << 21),
    //if rocket is inverted
    ERROR_ORIENTATION = (1 << 22),
    //warn
    WARN_BATT = (1 << 23),
    WARN_PYRO = (1 << 24),
    WARN_PYRO2 = (1 << 25),
    //FLIGHTPHASES 
    FLIGHTPHASE_BOOST = (1 << 26),
    FLIGHTPHASE_COAST = (1 << 27),
    FLIGHTPHASE_APOGEE = (1 << 28),
    FLIGHTPHASE_DROUGE_CHUTE = (1 << 29),
    FLIGHTPHASE_MAIN_CHUTE = (1 << 30)
    
};


