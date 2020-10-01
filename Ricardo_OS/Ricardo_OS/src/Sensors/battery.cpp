#include "battery.h"

#include "config.h"

#include "../Logging/messages.h"//this will likely be changed to be an object in the statemachine 


Battery::Battery(uint8_t pin)
{
    _pin = pin;
};

void Battery::setup(){
    //maybe some sort of calibration??
}

void Battery::update(){
    float factor = 6600/4095;

    batt_data.voltage = uint16_t(floor(float(factor*analogRead(_pin)))); // voltage in mV

    if (batt_data.voltage < warn_battery_voltage){
        new_message(WARN_BATT,"Battery at " + String(batt_data.voltage) + "mV"); 
    }else{
         delete_message(WARN_BATT);
    }


   batt_data.percent = uint16_t(((batt_data.voltage-empty_battery_voltage)/(full_battery_voltage-empty_battery_voltage)));
}
