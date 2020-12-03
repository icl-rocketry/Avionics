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
    gps(&(sm->I2C),&(sm->systemstatus)),
    baro(&(sm->vspi),&(sm->systemstatus)),
    imu(&(sm->vspi),&(sm->systemstatus)),
    batt(BattVolt,&(sm->systemstatus))
    
{
}

void Sensors::setup(){
    //calls setup for each indiviual sensor
    gps.setup();
    baro.setup();
    imu.setup();
    batt.setup();

    
};

void Sensors::update(){
//updates the raw_measuremets data structure with new measurements
    gps.update();
    baro.update();
    imu.update();
    batt.update();

    sensors_raw.ax = imu.imu_data.ax;
    sensors_raw.ay = imu.imu_data.ay;
    sensors_raw.az = imu.imu_data.az;
    sensors_raw.gx = imu.imu_data.gx;
    sensors_raw.gy = imu.imu_data.gy;
    sensors_raw.gz = imu.imu_data.gz;
    sensors_raw.mx = imu.imu_data.mx;
    sensors_raw.my = imu.imu_data.my;
    sensors_raw.mz = imu.imu_data.mz;

    //sensors_raw.batt_percent = batt.batt_data.percent;
    //sensors_raw.batt_volt = batt.batt_data.voltage;
    //sensors_raw.gps_lat = gps.gps_data.lat;
    //sensors_raw.gps_long = gps.gps_data.lng;
    /*
    Serial.println(imu.imu_data.ax);
    Serial.print("\t");
    Serial.print(imu.imu_data.ay);
    Serial.print("\t");
    Serial.print(imu.imu_data.az);
    Serial.print("\t");
    Serial.print(imu.imu_data.gx);
    Serial.print("\t");
    Serial.print(imu.imu_data.gy);
    Serial.print("\t");
    Serial.print(imu.imu_data.gz);
    Serial.print("\t");
    Serial.print(imu.imu_data.mx);
    Serial.print("\t");
    Serial.print(imu.imu_data.my);
    Serial.print("\t");
    Serial.print(imu.imu_data.mz);
    */
    

};