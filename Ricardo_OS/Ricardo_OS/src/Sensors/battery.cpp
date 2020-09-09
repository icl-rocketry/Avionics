#include "battery.h"
#include "config.h"

int battery_voltage(){
    float factor = 6600/4095;
    uint16_t voltage = floor(float(factor*analogRead(BattVolt))); // voltage in mV

    return voltage;
}

int battery_percentage(){return  uint8_t(battery_voltage()/full_battery_voltage);};
