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


#endif







