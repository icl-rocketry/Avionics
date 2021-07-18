#include "battery.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "flags.h"
#include "global_config.h"
#include <string>

#include "sensorStructs.h"

#include "Storage/utils.h"

Battery::Battery(uint8_t pin, SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data):

_systemstatus(systemstatus),
_logcontroller(logcontroller),
_raw_data(raw_data),
_pin(pin)


{};

void Battery::setup(){
    //maybe some sort of calibration??
}

void Battery::update(){
    
    const float factor = (1.0/0.488)*3300.0/4095.0; // esp32 pins can read upto 3.3v (3300mv) in 4095 steps, voltage divider halves the input voltage hence we get 2*3300 -> from actual board r1 = 9.22k, r2 = 8.79k
    uint16_t reading = analogRead(_pin);
    _raw_data->batt_volt = (uint16_t)(factor*(float)reading); // voltage in mV

    if ((_raw_data->batt_volt < warn_battery_voltage) && (!_systemstatus->flag_triggered(SYSTEM_FLAG::ERROR_BATT))){
        _systemstatus->new_message(SYSTEM_FLAG::ERROR_BATT,"Battery at " + utils::tostring(_raw_data->batt_volt) + "mV"); 
    }else if ((_raw_data->batt_volt > warn_battery_voltage) && (_systemstatus->flag_triggered(SYSTEM_FLAG::ERROR_BATT))){
         _systemstatus->delete_message(SYSTEM_FLAG::ERROR_BATT);
    }


   _raw_data->batt_percent = uint16_t(((float)(_raw_data->batt_volt-empty_battery_voltage)/(float)(full_battery_voltage-empty_battery_voltage))*100.0);
}
