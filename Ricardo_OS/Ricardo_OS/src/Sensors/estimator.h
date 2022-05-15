#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
#include "MadgwickAHRS.h"
#include "localizationkf.h"
#include "sensors.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include <ArduinoJson.h>

enum class ESTIMATOR_STATE: uint8_t{
    NOMINAL,
    PARTIAL,
    PARTIAL_NO_IMU,
    PARTIAL_NO_IMU_NO_GPS,
    PARTIAL_NO_IMU_NO_BARO,
    PARTIAL_NO_MAG,
    PARTIAL_NO_GPS,
    PARTIAL_NO_GPS_NO_BARO,
    PARTIAL_NO_BARO,
    NO_HOME,
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
        void resetLocalization();

        void setIgnitionTime(uint32_t time);
        void setLaunchTime(uint32_t time);
        void setApogeeTime(uint32_t time);

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
        static constexpr float g = 9.81;
        
        //POSITION ESTIMATION
        LocalizationKF localizationkf;

        
        //private methods

        void updateOrientation(const float &gx, const float &gy, const float &gz,
                               const float &ax, const float &ay, const float &az,
                               const float &mx, const float &my, const float &mz, float dt);

        void updateOrientation(const float &gx, const float &gy, const float &gz,
                               const float &ax, const float &ay, const float &az, float dt);

        void updateAngularRates(const float &gx, const float &gy, const float &gz);

        Eigen::Vector3f getLinearAcceleration(const float& ax,const float& ay, const float& az);

        void changeEstimatorState(ESTIMATOR_STATE state,std::string logmessage);

        /**
         * @brief generate prediction from localization kf and assign result to state
         * 
         * @param dt delta t in seconds
         */
        void predictLocalizationKF(const float& dt);
};


#endif