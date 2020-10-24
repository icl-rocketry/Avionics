#ifndef IMU_H
#define IMU_H
#include <Arduino.h>    
#include "SPI.h"
#include "Adafruit_LSM9DS1.h"

#include "Logging/systemstatus.h"

struct imu_data_t{
    float mx,my,mz;
    float ax,ay,az;
    float gx,gy,gz;
    //add more members if needed
};

class Imu{
    public:
        Imu(SPIClass* spi,SystemStatus* systemstatus);
        void setup();
        void update();

        imu_data_t imu_data;

    private:
        //pointer to spi object
        SPIClass* _spi;
        //pointer to system status object
        SystemStatus* _systemstatus;
        //adafruit imu object
        Adafruit_LSM9DS1 imu;

};    

#endif