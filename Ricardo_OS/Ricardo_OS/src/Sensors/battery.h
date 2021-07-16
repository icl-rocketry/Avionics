
#ifndef BATTERY_H
#define BATTERY_H
#include <Arduino.h>

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"


class Battery{
    public:
        Battery(uint8_t pin,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data);
        void setup();
        void update();
    
    private:
        //pointer to system status object
        SystemStatus* _systemstatus;
        LogController* _logcontroller;
        //pointer to raw data struct
        raw_measurements_t* _raw_data;

        uint8_t _pin;

        

};    

#endif