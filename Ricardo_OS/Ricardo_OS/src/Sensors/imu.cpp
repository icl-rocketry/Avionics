#include "imu.h"
#include "config.h"
#include "SPI.h"

#include "Adafruit_Sensor.h"
#include "ricardo_pins.h"

#include "Logging/systemstatus.h"
#include "flags.h"



Imu::Imu(SPIClass* spi, SystemStatus* systemstatus):
    imu(ImuCs, MagCs)
{
    _spi = spi;
    _systemstatus = systemstatus;
};

void Imu::setup(){
        if (!imu.begin()){
        _systemstatus->new_message(system_flag::ERROR_IMU, "Unable to initialize the imu");
    };
    //setup imu scaling
    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_16G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

};

void Imu::update(){};

