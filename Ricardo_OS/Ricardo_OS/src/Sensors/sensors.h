#ifndef SENSORS_H
#define SENSORS_H
//calls and updates raw sensor values

#include "Arduino.h"
#include "gps.h"



struct raw_measurements_t{
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    float gps_lat,gps_long,gps_speed,gps_alt;
    float baro_alt,baro_temp,baro_press;
    int batt_volt,batt_percent;
    uint32_t system_time;

};


class stateMachine; //forward declaration

class Sensors{
    public:
        Sensors(stateMachine* sm);
        void setup_sensors();
        void update();
        raw_measurements_t sensors_raw;
    private:
        stateMachine* _sm; //pointer to statemachine

        //individual sensor objects
        GPS gps;

};


#endif