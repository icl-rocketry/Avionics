#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
#include "MadgwickAHRS.h"
#include "localizationkf.h"
//#include "sensors.h"

enum class ESTIMATOR_STATE: uint8_t{
    NOMINAL,
    PARTIAL_GPS_IMU,
    PARTIAL_BARO_IMU,
    PARTIAL_IMU,
    PARTIAL_GPS_BARO,
    PARTIAL_GPS,
    PARTIAL_BARO,
    NOHOME,
    NOSOLUTION
};

class stateMachine; //forward declaration

class Estimator{
    public:
        Estimator(stateMachine* sm);   
        
        void setup();
        void update();

        void setHome(); //records the current position as the launch site
        bool isHomeSet(){return _homeSet;};
        
        void changeBeta(float beta); //remove this


        state_t state;
    private:
        stateMachine* _sm;//pointer to statemachine object
        //time variables
        unsigned long last_update;
        unsigned long update_frequency;

        bool _homeSet;
        
        
        //ORIENTATION ESTIMATION
        Madgwick madgwick; // madgwick filter object
        const float g  = 9.81; //the gravity constant which really isnt constant but oh well
        const Eigen::Vector3f gravity_vector{0,0,1};
        /**
         * @brief Flip z coordinate system
         * 
         */
        bool _upsideDown = false;
        float _flipConstant;
        
        //POSITION ESTIMATION
        LocalizationKF localizationkf;
        
        //private methods


        void updateAngularRates();
        void updateOrientation(float dt); // madgwick filter update
        void updateLinearAcceleration();

        void changeEstimatorState(ESTIMATOR_STATE state,std::string logmessage);
};


#endif