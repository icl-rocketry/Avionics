#include "imu.h"
#include "global_config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "flags.h"
#include "SparkFunLSM9DS1.h"

#include "sensorStructs.h"




Imu::Imu(SPIClass* spi, SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data):
    _spi(spi),
    _systemstatus(systemstatus),
    _logcontroller(logcontroller),
    imu(spi),
    _raw_data(raw_data),
    time_period(5) // correpsonds to 100hz
{};





void Imu::setup(){

    imu.setAccelScale(ACCEL_SCALE);
    //set samplerate of accel to 952Hz
    imu.settings.accel.sampleRate = 5;
    imu.settings.accel.enabled = true; // Enable accelerometer
    
    imu.setGyroScale(GYRO_SCALE);
    //set samplerate of gyro to 952Hz
    imu.settings.gyro.sampleRate = 5;
    imu.settings.gyro.lowPowerEnable = false;
    //imu.settings.accel.enabled = false; // Enable accelerometer
    // [HPFEnable] enables or disables the high-pass filter
    //imu.settings.gyro.HPFEnable = true; // HPF disabled
    // [HPFCutoff] sets the HPF cutoff frequency (if enabled)
    // Allowable values are 0-9. Value depends on ODR.
    // (Datasheet section 7.14)
    //imu.settings.gyro.HPFCutoff = 1; // HPF cutoff = 4Hz

    imu.setMagScale(MAG_SCALE);
    //imu.setMagScale(12);
    imu.settings.mag.XYPerformance = 3; // Ultra-high perform.
    imu.settings.mag.ZPerformance = 3; // Ultra-high perform.
    imu.settings.mag.sampleRate = 7;
    imu.settings.mag.lowPowerEnable = false;
    imu.settings.mag.operatingMode = 0; // Continuous mode
    //mag temp compensation -> this is a good thing right?
    imu.settings.mag.tempCompensationEnable = true;

    if (!imu.beginSPI(_SCLK,_MISO,_MOSI,ImuCs, MagCs)){
        _systemstatus->new_message(SYSTEM_FLAG::ERROR_IMU, "Unable to initialize the imu");
    };

};


void Imu::update(){

    read_gyro();
    read_accel();
    read_mag();
    read_temp();
       
   

};

void Imu::read_gyro(){
    imu.readGyro(); //degrees per second
    _raw_data->gx = imu.calcGyro(imu.gx);
    _raw_data->gy = imu.calcGyro(imu.gy);
    _raw_data->gz = imu.calcGyro(imu.gz);

}
void Imu::read_accel(){

    imu.readAccel();//g's
    _raw_data->ax = imu.calcAccel(imu.ax);
    _raw_data->ay = imu.calcAccel(imu.ay);
    _raw_data->az = imu.calcAccel(imu.az);
}
void Imu::read_mag(){
    imu.readMag(); 
    //conversion from Gauss to uT (microTesla) elon to the moon
    _raw_data->mx = imu.calcMag(imu.mx) * 100;
    _raw_data->my = imu.calcMag(imu.my) * 100;
    _raw_data->mz = imu.calcMag(imu.mz) * 100;

}
void Imu::read_temp(){
  if(imu.tempAvailable()){
        imu.readTemp();
        _raw_data->imu_temp = imu.temperature;
    }
}

void Imu::calibrateAccelGyro(bool autocalc){
    //4.56 0.76 0.45 521 87 51
    imu.calibrate(autocalc);
    /*
    Serial.print("Callibration Complete");
    Serial.print(imu.gBias[0]);
    Serial.print("\t");
    Serial.print(imu.gBias[1]);
    Serial.print("\t");
    Serial.print(imu.gBias[2]);
    Serial.print("\t");
    Serial.print(imu.gBiasRaw[0]);
    Serial.print("\t");
    Serial.print(imu.gBiasRaw[1]);
    Serial.print("\t");
    Serial.print(imu.gBiasRaw[2]);
    Serial.print("\n");*/
    

}

void Imu::calibrateMag(bool loadIn){
    imu.calibrateMag(loadIn);
}