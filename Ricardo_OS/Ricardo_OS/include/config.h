//Global Config
#ifndef CONFIG_H
#define CONFIG_H

//uncomment to show all debug messages
#define VERBOSE

//nominal charged battery voltage (mV)
#define full_battery_voltage 4200
#define warn_battery_voltage 3710 //coresponds to 15%
#define empty_battery_voltage 3270

//Serial baud rate
#define Serial_baud 115200
//I2C frequrency - 4Khz
#define I2C_FREQUENCY 400000
//I2C device addresses
//gps
#define I2C_GPS_ADDRESS 0x42
//SPI speed - 2MHz
#define SPI_CLOCK_DIV8 0x004c1001
//Lora Specific Config
#define LORA_SYNC_WORD 0xF3
//Lora region codes
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define LORA_REGION 866E6

#define ACCEL_SCALE 2
#define MAG_SCALE 4
#define GYRO_SCALE 245

//message flag definitions//
#define NONE 0 
//state flags
#define STATE_USBMODE (1 << 0) 
#define STATE_PREFLIGHT (1 << 1)
#define STATE_COUNTDOWN (1 << 2)
#define STATE_FLIGHT (1 << 3)
#define STATE_RECOVERY (1 << 4)
//critical messages 
#define ERROR_SPI (1 << 5)
#define ERROR_I2C (1 << 6)
#define ERROR_SERIAL (1 << 7)
#define ERROR_LORA (1 << 8)
#define ERROR_BARO (1 << 9)
#define ERROR_BATT (1 << 10)
#define ERROR_GPS (1 << 11)
#define ERROR_IMU (1 << 12)
#define ERROR_KALMAN (1 << 13)
#define ERROR_SD (1 << 14)
#define ERROR_FLASH (1 << 15)
//if rocket is inverted
#define ERROR_ORIENTATION (1 << 16)
//warn
#define WARN_BATT (1 << 17)
#define WARN_PYRO1 (1 << 18)
#define WARN_PYRO2 (1 << 19)
//info

//command flag definitions//
#define COMMAND_ABORT (1 << 0)


#endif







