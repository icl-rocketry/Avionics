#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
#include "MadgwickAHRS.h"
//#include "sensors.h"

enum class ESTIMATOR_STATE: uint8_t{
    NOMINAL,
    PARTIAL_GPS_IMU,
    PARTIAL_BARO_IMU,
    PARTIAL_IMU,
    PARTIAL_GPS_BARO,
    PARTIAL_GPS,
    PARTIAL_BARO,
    NOSOLUTION
};

class stateMachine; //forward declaration

class Estimator{
    public:
        Estimator(stateMachine* sm);   
        
        void setup();
        void update();

        void setHome(); // zeros sensors and records the current position as the launch site
        

        state_t state;
    private:
        stateMachine* _sm;//pointer to statemachine object
        //time variables
        unsigned long last_update;
        unsigned long update_frequency;
        
        

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
        void updateOrientation(float dt); // madgwick filter update
        void updateLinearAcceleration();

        void changeEstimatorState(ESTIMATOR_STATE state,std::string logmessage);
};


#endif