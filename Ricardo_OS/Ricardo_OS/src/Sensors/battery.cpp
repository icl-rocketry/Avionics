#include "battery.h"
#include "Logging/systemstatus.h"
#include "flags.h"
#include "config.h"



Battery::Battery(uint8_t pin, SystemStatus* systemstatus)
{
    _pin = pin;
    _systemstatus = systemstatus;
};

void Battery::setup(){
    //maybe some sort of calibration??
}

void Battery::update(){
    float factor = 6600/4095;

    batt_data.voltage = uint16_t(floor(float(factor*analogRead(_pin)))); // voltage in mV

    if (batt_data.voltage < warn_battery_voltage){
        _systemstatus->new_message(system_flag::WARN_BATT,"Battery at " + String(batt_data.voltage) + "mV"); 
    }else{
         _systemstatus->delete_message(system_flag::WARN_BATT);
    }


   batt_data.percent = uint16_t(((batt_data.voltage-empty_battery_voltage)/(full_battery_voltage-empty_battery_voltage)));
}
