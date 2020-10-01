
#ifndef BATTERY_H
#define BATTERY_H
#include <Arduino.h>

int battery_voltage();
int battery_percentage();


struct batt_data_t{
    uint16_t voltage,percent;
    //add more members if needed
};

class Battery{
    public:
        Battery(uint8_t pin);
        void setup();
        void update();

        batt_data_t batt_data;
    
    private:

        uint8_t _pin;

};    

#endif