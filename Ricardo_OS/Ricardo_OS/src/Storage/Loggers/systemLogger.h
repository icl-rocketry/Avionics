#pragma once

#include "logger.h"
#include <SdFat.h>
#include "config.h"
#include "../storageController.h"
#include <string>
#include "Sensors/sensorStructs.h"
#include "../logframe.h"
#include <vector>
#include <queue>
#include "flags.h"

class SystemLogger : public Logger{
    public:
        SystemLogger(StorageController* sc,
                    std::string filename,
                    STORAGE_DEVICE mainStorage = STORAGE_DEVICE::MICROSD,
                    STORAGE_DEVICE backupStorage = STORAGE_DEVICE::FLASH);

        void enable();
        void disable();

        void flush();

        void log(const std::string &message);
        void log(uint32_t status,uint32_t flag,std::string message);

        ~SystemLogger();
    private:
        system_logframe system_frame;
        std::queue<system_logframe> system_log_buffer;
        std::string raw_data_buffer;
        uint16_t raw_data_buffer_max_size = 5120; // max size 5kb

        std::string flagLevel(uint32_t flag);
        std::string flagLevel(SYSTEM_FLAG flag);

        uint16_t writeDelta;

        bool writeRawBuffer();



};

