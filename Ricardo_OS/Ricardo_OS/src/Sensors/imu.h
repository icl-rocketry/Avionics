#ifndef IMU_H
#define IMU_H
#include <Arduino.h>    
#include "SPI.h"
#include "Adafruit_LSM9DS1.h"
#include "SparkFunLSM9DS1.h"

#define RUAIRIDH_TEST_BOARD //used for testing  stuff

struct imu_data_t{
    float mx,my,mz;
    float ax,ay,az;
    float gx,gy,gz;
    //add more members if needed
};

class Imu{
    public:
        Imu(SPIClass* spi);
        void setup();
        void update();

        imu_data_t imu_data;

    private:
        //pointer to spi object
        SPIClass* _spi;
        void printAccel();
        #ifdef RUAIRIDH_TEST_BOARD
            //sparkfun imu object
            LSM9DS1 imu;
        #else
            //adafruit imu object
            Adafruit_LSM9DS1 imu;
        #endif

};    

#endif