#include "sensors.h"
#include "stateMachine.h"

#include <SPI.h>
#include <Wire.h>
#include "Storage/logController.h"
#include "Storage/systemstatus.h"


//config
#include "ricardo_pins.h"
#include "config.h"

//indivudal sensor classes

#include "max_m8q.h"
#include "ms5607.h"
#include "icm_20608.h"
#include "h3lis331dl.h"

#include "battery.h"

Sensors::Sensors(SPIClass& spi,TwoWire& I2C,SystemStatus& systemstatus,LogController& logcontroller) :
    gps(I2C,systemstatus,logcontroller),
    baro(spi,systemstatus,logcontroller,10),
    accelgyro(spi,systemstatus,logcontroller,10),
    accel(spi,systemstatus,logcontroller,10),
    mag(spi,systemstatus,logcontroller,10),
    batt(systemstatus,logcontroller,BattVolt)
    
{}

void Sensors::setup(){
    //calls setup for each indiviual sensor
    gps.setup();
    baro.setup();
    accelgyro.setup();
    accel.setup();
    batt.setup();
    
};

void Sensors::update()
{
    gps.update(sensors_raw.gps);
    baro.update(sensors_raw.baro);
    accelgyro.update(sensors_raw.accelgyro);
    accel.update(sensors_raw.accel);
    mag.update(sensors_raw.mag);

    batt.update(sensors_raw.batt);

    
};

const SensorStructs::raw_measurements_t& Sensors::getData()
{
    //TODO make this threadsafe maybe use a double buffer to make it lock free
    return sensors_raw;
}

void Sensors::calibrateAccelGyro()
{
    accelgyro.calibrateBias();
}

void Sensors::calibrateMag(MagCalibrationParameters magcal)
{
    mag.calibrate(magcal);
}

void Sensors::calibrateBaro()
{
    baro.calibrateBaro();
}
