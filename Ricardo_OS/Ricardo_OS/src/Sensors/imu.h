#ifndef IMU_H
#define IMU_H
#include <Arduino.h>    
#include "SPI.h"
#include "Adafruit_LSM9DS1.h"
#include "SparkFunLSM9DS1.h"

#define RUAIRIDH_TEST_BOARD //used for testing  stuff

struct imu_data_t{
    float gx,gy,gz;
    float ax,ay,az;
    float mx,my,mz;
    int16_t temperature;
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

        LSM9DS1 imu;

        void read_gyro();
        void read_mag();
        void read_accel();
        void read_temp();


};    

#endif