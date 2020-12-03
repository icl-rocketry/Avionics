#include "imu.h"
#include "config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "Logging/systemstatus.h"
#include "flags.h"
#include "SparkFunLSM9DS1.h"




Imu::Imu(SPIClass* spi, SystemStatus* systemstatus):
    _spi(spi),
    _systemstatus(systemstatus),
    imu(spi)
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
        imu_data.gx = imu.calcGyro(imu.gx);
        imu_data.gy = imu.calcGyro(imu.gy);
        imu_data.gz = imu.calcGyro(imu.gz);

    }
}
void Imu::read_accel(){
  if(imu.accelAvailable()){
        imu.readAccel();
        imu_data.ax = imu.calcAccel(imu.ax);
        imu_data.ay = imu.calcAccel(imu.ay);
        imu_data.az = imu.calcAccel(imu.az);

    }
}
void Imu::read_mag(){
  if(imu.magAvailable()){
        imu.readMag();
        imu_data.mx = imu.calcMag(imu.mx);
        imu_data.my = imu.calcMag(imu.my);
        imu_data.mz = imu.calcMag(imu.mz);
    }
}
void Imu::read_temp(){
  if(imu.tempAvailable()){
        imu.readTemp();
    }
}

