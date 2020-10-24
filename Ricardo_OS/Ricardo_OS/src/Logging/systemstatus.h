/*
handles all messages bitwise so multiple messages can be contained in a single variable


//reqwrite as class
*/

#ifndef MESSAGES_H
#define MESSAGES_H

#include "Arduino.h"

enum class system_flag {
    //state flags
    STATE_USBMODE = (1 << 0), 
    STATE_PREFLIGHT = (1 << 1),
    STATE_COUNTDOWN = (1 << 2),
    STATE_FLIGHT = (1 << 3),
    STATE_RECOVERY = (1 << 4),
    STATE_SETUP = (1 << 5),
    //critical messages 
    ERROR_SPI = (1 << 6),
    ERROR_I2C = (1 << 7),
    ERROR_SERIAL = (1 << 8),
    ERROR_LORA = (1 << 9),
    ERROR_BARO = (1 << 10),
    ERROR_BATT = (1 << 11),
    ERROR_GPS = (1 << 12),
    ERROR_IMU = (1 << 13),
    ERROR_KALMAN = (1 << 14),
    ERROR_SD = (1 << 15),
    ERROR_FLASH = (1 << 16),
    //if rocket is inverted
    ERROR_ORIENTATION = (1 << 17),
    //warn
    WARN_BATT = (1 << 18),
    WARN_PYRO1 = (1 << 19),
    WARN_PYRO2 = (1 << 20),
    //info 
}

class SystemStatus{
    public:
        SystemStatus();
        void new_message(system_flag flag,String reason);
        void delete_message(system_flag flag);
        bool flag_triggered(system_flag flag);
    protected:
        uint32_t _status;
}
#endif