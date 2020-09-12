//Global Config
#ifndef CONFIG_H
#define CONFIG_H

//uncomment to show all debug messages
#define VERBOSE

//nominal charged battery voltage (mV)
#define full_battery_voltage 4200

//message flag definitions
#define NONE 0
//state flags
#define STATE_USBMODE 1 
#define STATE_PREFLIGHT 2 
#define STATE_COUNTDOWN 4 
#define STATE_FLIGHT 8 
#define STATE_RECOVERY 16
//critical messages 
#define ERROR_SPI 32
#define ERROR_I2C 64
#define ERROR_SERIAL 128
#define ERROR_LORA 256
#define ERROR_BARO 512
#define ERROR_BATT 1024
#define ERROR_GPS 2048
#define ERROR_IMU 4096
#define ERROR_MAG 8192
#define ERROR_KALMAN 16384
#define ERROR_SD 32768
#define ERROR_FLASH 65536
//if rocket is inverted
#define ERROR_ORIENTATION 131072
//warn
#define WARN_BATT 262144

//info




#endif







