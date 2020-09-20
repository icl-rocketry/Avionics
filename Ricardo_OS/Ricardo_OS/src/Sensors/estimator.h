#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
//global struct for state of rocket
struct state_t{
    float lat,lng;
    float q[3]; //quaternion output
    float ypr[2]; //yaw pitch roll
    float x,y,z; //world coordinates(m) relative to launch position
    float vx,vy,vz; //rocket frame of reference velocity m/s
    float ax,ay,az; //rocket frame of reference aceleration m/s^2
    uint8_t estimator_state; 
};

struct raw_measurements_t{
    float ax,ay,az;
    float gx,gy,gz;
    float mx,my,mz;
    float gps_lat,gps_long,gps_speed,gps_alt;
    float baro_alt,baro_temp,baro_press;
    uint32_t system_time;

};

class Estimator{
    public:
        
        void setup();
        void update();

        state_t state;
        raw_measurements_t sensor_raw;


};


#endif