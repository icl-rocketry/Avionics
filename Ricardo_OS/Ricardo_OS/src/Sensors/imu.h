#ifndef IMU_H
#define IMU_H
#include <Arduino.h>    
#include "SPI.h"
#include "SparkFunLSM9DS1.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"

#include "magcalibration.h"


class Imu{
    public:
        Imu(SPIClass* spi,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data);
        void setup();
        void update();

        //SENSOR CALIBRATION
        void calibrateAccelGyro(bool autocalc); // autocalc automatically subtracts accel gyro biases
        void calibrateMag(bool save); // perform ellispoid fitting to calculate callibration coefficents and offset biases

    
    private:
        //pointer to spi object
        SPIClass* _spi;
        //pointer to system status object
        SystemStatus* _systemstatus;
        LogController* _logcontroller;
        //Sparkfun IMU object
        LSM9DS1 imu;
        //pointer to raw measurements struct
        raw_measurements_t* _raw_data;
        //mag calibration container
        MagCalibrationParameters _magCal;

        //functions for reading and converting read int16 values
        void read_gyro();
        void read_mag();
        void read_accel();
        void read_temp();
        /**
         * @brief write accel gyro bias callibration to nvs storage
         * 
         */
        void writeAccelGyroBias();
        /**
         * @brief Load accel gyro bias callibration from nvs storage
         * 
         */
        void loadAccelGyroBias();
        
        void writeMagCal();

        void loadMagCal();

};    

#endif