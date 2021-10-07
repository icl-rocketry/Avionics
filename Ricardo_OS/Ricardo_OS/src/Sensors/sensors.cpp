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
    gps(&(sm->I2C),&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    baro(&(sm->vspi),&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    imu(&(sm->vspi),&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    batt(BattVolt,&(sm->systemstatus),&(sm->logcontroller),&sensors_raw),
    _sm(sm)
    
{}

void Sensors::setup(){
    //calls setup for each indiviual sensor
    gps.setup();
    baro.setup();
    imu.setup();
    batt.setup();
    
};

void Sensors::update(){
//updates the raw_measuremets data structure with new measurements
    // uint32_t time = micros();
    
    gps.update();
    // Serial.println("gps:" + String((micros()-time)));
    // time =micros();
    baro.update();
    // Serial.println("baro:" + String((micros()-time)));
    // time =micros();
    imu.update();
    // Serial.println("imu:" + String((micros()-time)));
    // time =micros();
    batt.update();
    // Serial.println("batt:" + String((micros()-time)));
    // time =micros();
    
};