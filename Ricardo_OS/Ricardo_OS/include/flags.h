#ifndef FLAGS_H
#define FLAGS_H

enum class system_flag:uint32_t{
    //state flags
    STATE_USBMODE = (1 << 0), 
    STATE_PREFLIGHT = (1 << 1),
    STATE_COUNTDOWN = (1 << 2),
    STATE_FLIGHT = (1 << 3),
    STATE_RECOVERY = (1 << 4),
    STATE_SETUP = (1 << 5),
    STATE_GROUNDSTATION = (1 << 6),
    //critical messages 
    ERROR_SPI = (1 << 7),
    ERROR_I2C = (1 << 8),
    ERROR_SERIAL = (1 << 9),
    ERROR_LORA = (1 << 10),
    ERROR_BARO = (1 << 11),
    ERROR_BATT = (1 << 12),
    ERROR_GPS = (1 << 13),
    ERROR_IMU = (1 << 14),
    ERROR_KALMAN = (1 << 15),
    ERROR_SD = (1 << 16),
    ERROR_FLASH = (1 << 17),
    ERROR_CAN = (1 << 18),
    //if rocket is inverted
    ERROR_ORIENTATION = (1 << 19),
    //warn
    WARN_BATT = (1 << 20),
    WARN_PYRO1 = (1 << 21),
    WARN_PYRO2 = (1 << 22),
    //info 
};

#endif