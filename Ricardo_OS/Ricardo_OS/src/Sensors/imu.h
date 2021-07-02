#ifndef IMU_H
#define IMU_H
#include <Arduino.h>    
#include "SPI.h"
#include "SparkFunLSM9DS1.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"


class Imu{
    public:
        Imu(SPIClass* spi,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data);
        void setup();
        void update();

        //BIAS CALIBRATION
        void calibrateAccelGyro(bool autocalc); // autocalc automatically subtracts accel gyro biases
        void calibrateMag(bool loadIn); // loadIn loads mag biases into mag registers -> this is only a bias correction 

    
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

        //functions for reading and converting read int16 values
        void read_gyro();
        void read_mag();
        void read_accel();
        void read_temp();


};    

#endif