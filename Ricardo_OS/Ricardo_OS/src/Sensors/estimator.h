#include <Arduino.h>

#ifndef ESTIMATOR_H
#define ESTIMATOR_H

//global struct for state of rocket
struct state_t{
    float lat,lng;
    float q[3]; //quaternion output
    float ypr[2]; //yaw pitch roll
    float x,y,z; //world coordinates(m) relative to launch position
    float vx,vy,vz; //rocket frame of reference velocity m/s
    float ax,ay,az; //rocket frame of reference aceleration m/s^2
    uint8_t estimator_state; 
}state;

void setup_estimator();
void update_estimator();

#endif