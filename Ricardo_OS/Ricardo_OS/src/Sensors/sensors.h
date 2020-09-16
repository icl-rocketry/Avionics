/* 
setup internal spi and i2c 

Kiran de Silva
*/

#include "Arduino.h"

#ifndef SENSORS_H
#define SENSORS_H

struct raw_measurements_t{
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    float gps_lat,gps_long,gps_speed,gps_alt;
    float baro_alt,baro_temp,baro_press;
    uint32_t time;

}sensors_raw;

void setup_sensors();
void update_sensors_raw();

#endif