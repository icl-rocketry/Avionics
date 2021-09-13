#pragma once

#include "logger.h"
#include <SdFat.h>
#include "config.h"
#include "../storageController.h"
#include <string>
#include "Sensors/sensorStructs.h"
#include "../logframe.h"
#include <vector>
#include <array>

class TelemetryLogger : public Logger
{
public:
    TelemetryLogger(StorageController* sc,
                    uint16_t dt,
                    std::string filename,
                    STORAGE_DEVICE mainStorage = STORAGE_DEVICE::MICROSD,
                    STORAGE_DEVICE backupStorage = STORAGE_DEVICE::FLASH);

    void enable();
    void disable();

    void writeLog();

    void log(state_t &estimator_state,raw_measurements_t &raw_sensors);

    ~TelemetryLogger();
private:
    void flush();
    telemetry_logframe telemetry_frame;
    //std::vector<telemetry_logframe> telemetry_log_buffer;
    //std::vector<uint8_t> telemetry_log_buffer;
    std::string telemetry_log_buffer;
};

