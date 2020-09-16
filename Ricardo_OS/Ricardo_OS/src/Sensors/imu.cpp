#include "imu.h"
#include "config.h"
#include "SPI.h"
#include "Adafruit_LSM9DS1.h"
#include "Adafruit_Sensor.h"
#include "ricardo_pins.h"
#include "../Logging/messages.h"

Adafruit_LSM9DS1 imu = Adafruit_LSM9DS1(ImuCs, MagCs);

void setup_imu(){

    if (!imu.begin()){
        new_message(ERROR_IMU, "Unable to initialize the imu");
    };
    //setup imu scaling
    imu.setupAccel(imu.LSM9DS1_ACCELRANGE_2G);
    imu.setupMag(imu.LSM9DS1_MAGGAIN_4GAUSS);
    imu.setupGyro(imu.LSM9DS1_GYROSCALE_245DPS);

};

