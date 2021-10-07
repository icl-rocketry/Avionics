
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

        
        /**
         * @brief esp32 pins can read upto 3.3v (3300mv) in 4095 steps, voltage divider halves the input voltage hence we get 2*3300 -> from actual board r1 = 9.22k, r2 = 8.79k
         * 
         */

        static constexpr float factor = (1.0/0.488)*3300.0/4095.0; 

        uint8_t _pin;

        uint16_t sampleDelta = 200; // sample the battery voltage at 5hz
        uint32_t prevSampleTime =0;

        

};    

#endif