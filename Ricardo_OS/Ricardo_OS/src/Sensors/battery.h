
#ifndef BATTERY_H
#define BATTERY_H
#include <Arduino.h>

#include "Logging/systemstatus.h"

#include "sensorStructs.h"


class Battery{
    public:
        Battery(uint8_t pin,SystemStatus* systemstatus,raw_measurements_t* raw_data);
        void setup();
        void update();
    
    private:
        //pointer to system status object
        SystemStatus* _systemstatus;
        //pointer to raw data struct
        raw_measurements_t* _raw_data;

        uint8_t _pin;

};    

#endif