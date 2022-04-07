
#pragma once

#include <Arduino.h>

#include "Storage/systemstatus.h"
#include "Storage/logController.h"
#include "sensorStructs.h"

//needs to be rewritten as a battery handler as it needs to be able to handle 
//multiple different power type
//maybe average the different power levels?



class Battery
{
public:
    Battery(SystemStatus &systemstatus, LogController &logcontroller, uint8_t pin);
    void setup();
    void update(SensorStructs::BATT_t &data);

private:
    SystemStatus &_systemstatus;
    LogController &_logcontroller;
    const uint8_t _pin;

    /**
     * @brief esp32 pins can read upto 3.3v (3300mv) in 4095 steps, voltage divider halves the input voltage hence we get 2*3300 -> from actual board r1 = 9.22k, r2 = 8.79k
     *
     */
    static constexpr float factor = (1.f / 0.488f) * 3300.f / 4095.f;

    uint16_t sampleDelta = 200; // sample the battery voltage at 5hz
    uint32_t prevSampleTime = 0;
};

