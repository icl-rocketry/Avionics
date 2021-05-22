#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
#include "MadgwickAHRS.h"
//#include "sensors.h"





class stateMachine; //forward declaration

class Estimator{
    public:
        Estimator(stateMachine* sm);   
        
        void setup();
        void update();

        state_t state;
    private:
        stateMachine* _sm;//pointer to statemachine object
        //time variables
        unsigned long last_update;
        unsigned long update_frequency;
        unsigned long dt;
        float dt_seconds;

        Madgwick madgwick; // madgwick filter object
        const float g  = 9.81; //the gravity constant which really isnt constant but oh well
        const Eigen::Matrix<float,3,1> gravity_vector{0,0,1};
        //lsm9ds1 z axis points up 
        //depending on the orientation of ricardo we may need to "flip "the gravitational vector
        //if ric is oriented with imu facing upwards, gravity acts in the negative z direction
        //hence we should add the gravity vector to remove gravtiy from the readings.
        bool upsideDown = false;
        float flipConstant;
        
        //private methods


        void updateAngularRates();
        void updateOrientation();
        void updateLinearAcceleration();
};


#endif