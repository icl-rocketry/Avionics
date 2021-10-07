#include "battery.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "flags.h"
#include "global_config.h"
#include <string>

#include "sensorStructs.h"


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

    if (millis()-prevSampleTime >= sampleDelta){
    
        
        
        const uint16_t reading = analogRead(_pin);
        _raw_data->batt_volt = (uint16_t)(factor*(float)reading); // voltage in mV

        if ((_raw_data->batt_volt < warn_battery_voltage) && (!_systemstatus->flag_triggered(SYSTEM_FLAG::ERROR_BATT))){
            _systemstatus->new_message(SYSTEM_FLAG::ERROR_BATT,"Battery at " + std::to_string(_raw_data->batt_volt) + "mV"); 
        }else if ((_raw_data->batt_volt > warn_battery_voltage) && (_systemstatus->flag_triggered(SYSTEM_FLAG::ERROR_BATT))){
            _systemstatus->delete_message(SYSTEM_FLAG::ERROR_BATT);
        }


        _raw_data->batt_percent = uint16_t(((float)(_raw_data->batt_volt-empty_battery_voltage)/(float)(full_battery_voltage-empty_battery_voltage))*100.0);
    }
}
