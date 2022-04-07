#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
#include "MadgwickAHRS.h"
#include "localizationkf.h"
#include "sensors.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"

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


class Estimator{
    public:
        Estimator(SystemStatus& systemstatus,LogController& logcontroller);   
        
        void setup();
        void update(const SensorStructs::raw_measurements_t& raw_sensors);

        void setHome(const SensorStructs::raw_measurements_t& raw_sensors); //records the current position as the launch site
        bool isHomeSet(){return _homeSet;};
        
        void changeBeta(float beta);
        void resetOrientation();

        const SensorStructs::state_t& getData();

        
    private:
        // stateMachine* _sm;//pointer to statemachine object
        SystemStatus& _systemstatus;
        LogController& _logcontroller;

        SensorStructs::state_t state;

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


        void updateAngularRates(const float& gx,const float& gy,const float& gz);
        void updateOrientation(const float& gx,const float& gy,const float& gz,const float& ax,const float& ay,const float& az,const float& mx,const float& my,const float& mz,float dt); // madgwick filter update
        void updateLinearAcceleration(const float& ax,const float& ay,const float& az);

        void changeEstimatorState(ESTIMATOR_STATE state,std::string logmessage);
};


#endif