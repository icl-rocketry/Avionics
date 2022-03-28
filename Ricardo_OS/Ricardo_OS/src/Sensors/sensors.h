#ifndef SENSORS_H
#define SENSORS_H
//calls and updates raw sensor values

#include "Arduino.h"

#include "sensorStructs.h"


#include "gps.h"
#include "baro.h"
#include "imu.h"
#include "battery.h"



class stateMachine; //forward declaration

class Sensors{
    public:
        Sensors(stateMachine* sm);

        void setup();
        void update();

        void enableHITL();
        void disableHITL();
        void updateHITL();

        raw_measurements_t const * getRaw();

        raw_measurements_t sensors_raw;

        GPS gps;
        Baro baro;
        Imu imu;
        Battery batt;

    private:
        stateMachine* _sm; //pointer to statemachine

    
};


#endif