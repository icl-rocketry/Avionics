#include "imu.h"
#include "global_config.h"
#include "SPI.h"
#include "ricardo_pins.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "flags.h"
#include "SparkFunLSM9DS1.h"

#include "sensorStructs.h"

#include "Preferences.h"


#include <Eigen/Core>




Imu::Imu(SPIClass* spi, SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data):
    _spi(spi),
    _systemstatus(systemstatus),
    _logcontroller(logcontroller),
    imu(spi),
    _raw_data(raw_data),
    _magCal{1,
           0,
           0,
           Eigen::Matrix3f{{1,0,0},{0,1,0},{0,0,1}},
           Eigen::Vector3f{{0,0,0}}} // default for mag biases
{};





void Imu::setup(){

    imu.setAccelScale(ACCEL_SCALE);
    //set samplerate of accel to 476Hz
    imu.settings.accel.sampleRate = 5;
    imu.settings.accel.enabled = true; // Enable accelerometer
    
    imu.setGyroScale(GYRO_SCALE);
    //set samplerate of gyro to 476Hz
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

    loadAccelGyroBias(); //load previously callibrated bias values from nvs
    loadMagCal(); //load mag calibration coefficents from nvs

    if (!imu.beginSPI(_SCLK,_MISO,_MOSI,ImuCs, MagCs)){
        _systemstatus->new_message(SYSTEM_FLAG::ERROR_IMU, "Unable to initialize the imu");
        return;
    };
    _logcontroller->log("IMU Initialized");

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
    _raw_data->gy = -imu.calcGyro(imu.gy);
    _raw_data->gz = imu.calcGyro(imu.gz);

}
void Imu::read_accel(){

    imu.readAccel();//g's
    _raw_data->ax = imu.calcAccel(imu.ax);
    _raw_data->ay = -imu.calcAccel(imu.ay);
    _raw_data->az = imu.calcAccel(imu.az);
 }
void Imu::read_mag(){
    imu.readMag(); 
    //Gauss
    float mx = -imu.calcMag(imu.mx);
    float my = -imu.calcMag(imu.my);
    float mz = imu.calcMag(imu.mz);
    Eigen::Vector3f corrected_mag = _magCal.A_1*(Eigen::Vector3f{{mx,my,mz}} - _magCal.b);
    _raw_data->mx = corrected_mag[0];
    _raw_data->my = corrected_mag[1];
    _raw_data->mz = corrected_mag[2];
    

}
void Imu::read_temp(){
    imu.readTemp();
    _raw_data->imu_temp = imu.temperature;
}

void Imu::calibrateAccelGyroBias(bool autocalc){
    //4.56 0.76 0.45 521 87 51
    imu.calibrate(autocalc);
    writeAccelGyroBias(); // write bias offsets to nvs
    _logcontroller->log("IMU accel gyro bias callibration complete");
}

void Imu::calibrateMagBias(bool loadIn){ // simple bias correction. 
    imu.calibrateMag(loadIn);
    _logcontroller->log("IMU simple mag bias callibration complete");
}

void Imu::calibrateMagFull(MagCalibrationParameters magCal) 
{
    _magCal = magCal;
    writeMagCal();
}

void Imu::writeAccelGyroBias(){
    Preferences pref;

    if (!pref.begin("IMU")){
        _logcontroller->log("nvs failed to start. Can't write calbration offsets");
        return;
    }   
    //dont have time to write a new library for this
    //i dont like the way we cant log the errors to our log file as the error handling
    //is wrapped up in the preferences class
    // it shouldnt be too hard to rewrite at a later date
    if (!pref.putShort("gxBias",imu.gBiasRaw[0])){_logcontroller->log("nvs error while writing");};
    if (!pref.putShort("gyBias",imu.gBiasRaw[1])){_logcontroller->log("nvs error while writing");};
    if (!pref.putShort("gzBias",imu.gBiasRaw[2])){_logcontroller->log("nvs error while writing");};
    if (!pref.putShort("axBias",imu.aBiasRaw[0])){_logcontroller->log("nvs error while writing");};
    if (!pref.putShort("ayBias",imu.aBiasRaw[1])){_logcontroller->log("nvs error while writing");};
    if (!pref.putShort("azBias",imu.aBiasRaw[2])){_logcontroller->log("nvs error while writing");};
    //preferences end is called in destructor of preference class  as it goes out of scope

}

void Imu::loadAccelGyroBias(){
    Preferences pref;

    if (!pref.begin("IMU",true)){
        _logcontroller->log("nvs failed to start");
        return;
    }  

    imu.gBiasRaw[0] = pref.getShort("gxBias");
    imu.gBiasRaw[1] = pref.getShort("gyBias");
    imu.gBiasRaw[2] = pref.getShort("gzBias");
    imu.aBiasRaw[0] = pref.getShort("axBias");
    imu.aBiasRaw[1] = pref.getShort("ayBias");
    imu.aBiasRaw[2] = pref.getShort("azBias");


}

void Imu::writeMagCal() 
{
    Preferences pref;

    if (!pref.begin("IMU")){
        _logcontroller->log("nvs failed to start. Can't write calbration offsets");
        return;
    }   

    if (!pref.putFloat("F",_magCal.fieldMagnitude)){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("I",_magCal.inclination)){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("D",_magCal.declination)){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A11",_magCal.A_1(0,0))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A12",_magCal.A_1(0,1))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A12",_magCal.A_1(0,2))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A21",_magCal.A_1(1,0))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A22",_magCal.A_1(1,1))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A22",_magCal.A_1(1,2))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A31",_magCal.A_1(2,0))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A32",_magCal.A_1(2,1))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("A32",_magCal.A_1(2,2))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("b1",_magCal.b(0))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("b2",_magCal.b(1))){_logcontroller->log("nvs error while writing");};
    if (!pref.putFloat("b3",_magCal.b(2))){_logcontroller->log("nvs error while writing");};
    
 

}

void Imu::loadMagCal() 
{
    Preferences pref;

    if (!pref.begin("IMU",true)){
        _logcontroller->log("nvs failed to start");
        return;
    }  

    _magCal.fieldMagnitude = pref.getFloat("F",1);
    _magCal.inclination = pref.getFloat("I",0);
    _magCal.declination = pref.getFloat("D",0);

    _magCal.A_1 << pref.getFloat("A11",1),pref.getFloat("A12",0),pref.getFloat("A13",0),
                   pref.getFloat("A21",0),pref.getFloat("A22",1),pref.getFloat("A23",0),
                   pref.getFloat("A31",0),pref.getFloat("A32",0),pref.getFloat("A33",1);

    _magCal.b << pref.getFloat("b1",0),pref.getFloat("b2",0),pref.getFloat("b3",0);
}