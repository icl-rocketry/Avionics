#include "imu.h"
#include "config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "Logging/systemstatus.h"
#include "flags.h"
#include "SparkFunLSM9DS1.h"

#include "sensorStructs.h"




Imu::Imu(SPIClass* spi, SystemStatus* systemstatus,raw_measurements_t* raw_data):
    _spi(spi),
    _systemstatus(systemstatus),
    imu(spi),
    _raw_data(raw_data)
{};





void Imu::setup(){

    imu.setAccelScale(ACCEL_SCALE);
    imu.setGyroScale(GYRO_SCALE);
    imu.setMagScale(MAG_SCALE);

    if (!imu.beginSPI(_SCLK, _MISO, _MOSI, ImuCs, MagCs)){
        _systemstatus->new_message(system_flag::ERROR_IMU, "Unable to initialize the imu");
    };

};


void Imu::update(){
    read_gyro();
    read_accel();
    read_mag();
    read_temp();
};

void Imu::read_gyro(){
  if(imu.gyroAvailable()){
        imu.readGyro();
        _raw_data->gx = imu.calcGyro(imu.gx);
        _raw_data->gy = imu.calcGyro(imu.gy);
        _raw_data->gz = imu.calcGyro(imu.gz);

    }
}
void Imu::read_accel(){
  if(imu.accelAvailable()){
        imu.readAccel();
        _raw_data->ax = imu.calcAccel(imu.ax);
        _raw_data->ay = imu.calcAccel(imu.ay);
        _raw_data->az = imu.calcAccel(imu.az);

    }
}
void Imu::read_mag(){
  if(imu.magAvailable()){
        imu.readMag();
        _raw_data->mx = imu.calcMag(imu.mx);
        _raw_data->my = imu.calcMag(imu.my);
        _raw_data->mz = imu.calcMag(imu.mz);
    }
}
void Imu::read_temp(){
  if(imu.tempAvailable()){
        imu.readTemp();
        _raw_data->imu_temp = imu.temperature;
    }
}

