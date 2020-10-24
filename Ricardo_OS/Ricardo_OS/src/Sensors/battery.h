
#ifndef BATTERY_H
#define BATTERY_H
#include <Arduino.h>

#include "Logging/systemstatus.h"


struct batt_data_t{
    uint16_t voltage,percent;
    //add more members if needed
};

class Battery{
    public:
        Battery(uint8_t pin,SystemStatus* systemstatus);
        void setup();
        void update();

        batt_data_t batt_data;
    
    private:
        //pointer to system status object
        SystemStatus* _systemstatus;

        uint8_t _pin;

};    

#endif