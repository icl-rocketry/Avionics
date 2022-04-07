#include "battery.h"
#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "flags.h"
#include "global_config.h"
#include <string>

#include "sensorStructs.h"


Battery::Battery(SystemStatus& systemstatus,LogController& logcontroller,uint8_t pin):
_systemstatus(systemstatus),
_logcontroller(logcontroller),
_pin(pin)
{};

void Battery::setup(){
    //todo -> add configuration to allow communication with pdus as a network sensor
}

void Battery::update(SensorStructs::BATT_t &data)
{

    if (millis() - prevSampleTime >= sampleDelta)
    {

        const uint16_t reading = analogRead(_pin);
        data.volt = (uint16_t)(factor * (float)reading); // voltage in mV

        if ((data.volt < warn_battery_voltage) && (!_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BATT)))
        {
            _systemstatus.new_message(SYSTEM_FLAG::ERROR_BATT, "Battery at " + std::to_string(data.volt) + "mV");
        }
        else if ((data.volt > warn_battery_voltage) && (_systemstatus.flag_triggered(SYSTEM_FLAG::ERROR_BATT)))
        {
            _systemstatus.delete_message(SYSTEM_FLAG::ERROR_BATT);
        }

        data.percent = uint16_t(((float)(data.volt - empty_battery_voltage) / (float)(full_battery_voltage - empty_battery_voltage)) * 100.0);
    }
}
