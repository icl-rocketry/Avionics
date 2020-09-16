#include "battery.h"
#include "config.h"
#include "ricardo_pins.h"
#include "../Logging/messages.h"

int battery_voltage(){
    float factor = 6600/4095;
    uint16_t voltage = floor(float(factor*analogRead(BattVolt))); // voltage in mV
    if (voltage < warn_battery_voltage){
        new_message(WARN_BATT,"Battery at " + String(voltage) + "mV"); 
    }else{
         delete_message(WARN_BATT);
    }
    return voltage;
}

int battery_percentage(){return  int8_t((battery_voltage()-empty_battery_voltage)/(full_battery_voltage-empty_battery_voltage));};
