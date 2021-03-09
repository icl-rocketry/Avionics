#include "battery.h"
#include "Logging/systemstatus.h"
#include "flags.h"
#include "config.h"
#include <string>

#include "sensorStructs.h"

#include "Logging/utils.h"

Battery::Battery(uint8_t pin, SystemStatus* systemstatus,raw_measurements_t* raw_data):

_systemstatus(systemstatus),
_raw_data(raw_data),
_pin(pin)


{};

void Battery::setup(){
    //maybe some sort of calibration??
}

void Battery::update(){
    
    float factor = 6600/4095;

    _raw_data->batt_volt = uint16_t(floor(float(factor*analogRead(_pin)))); // voltage in mV

    if ((_raw_data->batt_volt < warn_battery_voltage) && (!_systemstatus->flag_triggered(system_flag::WARN_BATT))){
        _systemstatus->new_message(system_flag::WARN_BATT,"Battery at " + utils::tostring(_raw_data->batt_volt) + "mV"); 
    }else if ((_raw_data->batt_volt > warn_battery_voltage) && (_systemstatus->flag_triggered(system_flag::WARN_BATT))){
         _systemstatus->delete_message(system_flag::WARN_BATT);
    }


   _raw_data->batt_percent = uint16_t(((_raw_data->batt_volt-empty_battery_voltage)/(full_battery_voltage-empty_battery_voltage)));
}
