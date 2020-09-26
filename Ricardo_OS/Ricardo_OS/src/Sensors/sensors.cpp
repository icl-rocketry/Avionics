
#include "sensors.h"
#include "ricardo_pins.h"
#include "config.h"
#include "internal_io.h"

#include "gps.h"
#include "imu.h"
#include "baro.h"
#include "battery.h"






void Sensors::setup_sensors(){
    //calls setup for each indiviual sensor
    setup_gps();
    setup_baro();
    setup_imu();
    
};

void Sensors::update(){
//updates the raw_measuremets data structure with new measurements

    sensors_raw.batt_percent = battery_percentage();
    sensors_raw.batt_volt = battery_voltage();


};