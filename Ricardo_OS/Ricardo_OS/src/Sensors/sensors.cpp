#include "sensors.h"
#include "stateMachine.h"



//config
#include "ricardo_pins.h"
#include "config.h"

//indivudal sensor classes

#include "gps.h"
#include "imu.h"
#include "baro.h"
#include "battery.h"

Sensors::Sensors(stateMachine* sm) :
    _sm(sm),
    gps(&(sm->I2C),&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    baro(&(sm->vspi),&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    imu(&(sm->vspi),&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    batt(BattVolt,&(sm->systemstatus),&(sm->logcontroller),&sensors_raw)
    
{
}

void Sensors::setup(){
    //calls setup for each indiviual sensor
    gps.setup();
    baro.setup();
    imu.setup();
    batt.setup();
    
};

void Sensors::calibrate(SENSOR sensor){
    switch(sensor) {
        case SENSOR::ACCELGYRO:
        {
            imu.calibrateAccelGyro(true); // this is performed while the gyro and accel is STABLE and FLAT
            _sm->logcontroller.log("IMU accel gyro bias callibration complete");
            break;
        }
        case SENSOR::MAG:
        {
            imu.calibrateMag(true); // wave ricardo in the air like you just dont care or in a figure of eight up to you although i know which one i would do...
            _sm->logcontroller.log("IMU mag bias callibration complete");
            break;
        }
        case SENSOR::BARO:
        {
            break;
        }
        case SENSOR::GPS:
        {
            break;
        }
        case SENSOR::BATT:
        {
            break;
        }
        default:
        //do nothing 
        break;
    }
}

void Sensors::update(){
//updates the raw_measuremets data structure with new measurements
    gps.update();
    baro.update();
    imu.update();
    batt.update();
    
};