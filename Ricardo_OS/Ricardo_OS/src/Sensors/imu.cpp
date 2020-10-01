#include "imu.h"
#include "config.h"
#include "SPI.h"

#include "Adafruit_Sensor.h"
#include "ricardo_pins.h"
#include "../Logging/messages.h"



Imu::Imu(SPIClass* spi):
    imu(ImuCs, MagCs)
{
    _spi = spi;
};

void Imu::setup(){
        if (!imu.begin()){
        new_message(ERROR_IMU, "Unable to initialize the imu");
    };
    //setup imu scaling
    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

};

void Imu::update(){};

