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

void Sensors::callibrate(SENSOR sensor){
    switch(sensor) {
        case SENSOR::ACCEL:
        {
        
        }
        case SENSOR::MAG:
        {
        
        }
        case SENSOR::GYRO:
        {
        
        }
        case SENSOR::BARO:
        {
        
        }
        case SENSOR::GPS:
        {
        
        }
        case SENSOR::BATT:
        {
        
        }
    }
}

void Sensors::update(){
//updates the raw_measuremets data structure with new measurements
    gps.update();
    baro.update();
    imu.update();
    batt.update();
    
};