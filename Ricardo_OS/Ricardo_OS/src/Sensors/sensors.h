#ifndef SENSORS_H
#define SENSORS_H
//calls and updates raw sensor values

#include "Arduino.h"

#include "sensorStructs.h"


#include "gps.h"
#include "baro.h"
#include "imu.h"
#include "battery.h"


enum class SENSOR:uint8_t{
    ACCEL,
    GYRO,
    ACCELGYRO, // subroutines such as the callibration are perfomed simultaneously for both
    MAG,  
    BARO,
    GPS,
    BATT
};


class stateMachine; //forward declaration

class Sensors{
    public:
        Sensors(stateMachine* sm);

        void setup();
        void update();
        void callibrate(SENSOR sensor);

        raw_measurements_t sensors_raw;


    private:
        stateMachine* _sm; //pointer to statemachine

        //individual sensor objects
        GPS gps;
        Baro baro;
        Imu imu;
        
        Battery batt;


};


#endif