//Global Config
#ifndef GLOBALCONFIG_H
#define GLOBALCONFIG_H

//uncomment to show all debug messages
#define VERBOSE

//nominal charged battery voltage (mV)
#define full_battery_voltage 4200
#define warn_battery_voltage 3710 //coresponds to 15%
#define empty_battery_voltage 3270

//Serial baud rate
#define Serial_baud 115200
//Serial rx buffer size
#define SERIAL_SIZE_RX 256
//Serial max timeout
#define MAX_SERIAL_TIMEOUT 5
//I2C frequrency - 4Khz
#define I2C_FREQUENCY 400000
//I2C device addresses
//gps
#define I2C_GPS_ADDRESS 0x42
//gps specific registers
#define GPS_NUM_AVAILABLE_BYTES_REGISTER 0xfd
#define GPS_DATASTREAM_REGISTER 0xff
//SPI speed - 2MHz
#define SPI_CLOCK_DIV8 0x004c1001
//Lora Specific Config
#define LORA_SYNC_WORD 0xF3
//Lora region codes
//433E6 for Asia
//866E6 for Europe
//915E6 for North America
#define LORA_REGION 866E6
//define max packet length on any interface in the network
#define MAX_PACKET_LENGTH 256

#define ACCEL_SCALE 2
#define MAG_SCALE 12
#define GYRO_SCALE 245

//ArduinoJson
#define ARDUINOJSON_ENABLE_COMMENTS 1
#endif







