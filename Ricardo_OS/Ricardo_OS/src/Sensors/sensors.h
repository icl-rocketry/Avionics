#pragma once
/**
 * @file sensors.h
 * @author Kiran de Silva
 * @brief Manages sensor suite on avionics.
 * TODO:
 * Threadsafe
 * @version 0.1
 * @date 2022-04-06
 * 
 * @copyright Copyright (c) 2022
 * 
 */


#include "sensorStructs.h"

#include <SPI.h>
#include <Wire.h>
#include "Storage/logController.h"
#include "Storage/systemstatus.h"


#include "max_m8q.h"
#include "ms5607.h"
#include "icm_20608.h"
#include "h3lis331dl.h"
#include "mmc5983ma.h"


#include "battery.h"

class Sensors
{
public:
    Sensors(SPIClass &spi, TwoWire &I2C, SystemStatus &systemstatus, LogController &logcontroller);

    void setup();
    void update();

    /**
     * @brief Get the Raw Sensor Data
     *
     * @return const SensorStructs::raw_measurements_t&
     */
    const SensorStructs::raw_measurements_t &getData();

    // Sensor Calibration Functions
    void calibrateAccelGyro();
    void calibrateMag(MagCalibrationParameters magcal);
    void calibrateBaro();

private:
    SensorStructs::raw_measurements_t sensors_raw;

    Max_M8Q gps;
    MS5607 baro;
    ICM_20608 accelgyro;
    H3LIS331DL accel;
    MMC5983MA mag;
    Battery batt;

    


};
