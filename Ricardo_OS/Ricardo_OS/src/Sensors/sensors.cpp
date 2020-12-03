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