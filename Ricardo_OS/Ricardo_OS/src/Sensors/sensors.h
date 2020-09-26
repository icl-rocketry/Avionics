#ifndef SENSORS_H
#define SENSORS_H
//calls and updates raw sensor values

#include "Arduino.h"



struct raw_measurements_t{
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    float gps_lat,gps_long,gps_speed,gps_alt;
    float baro_alt,baro_temp,baro_press;
    int batt_volt,batt_percent;
    uint32_t system_time;

};



class Sensors{
    public:
        void setup_sensors();
        void update();
        raw_measurements_t sensors_raw;


};


#endif