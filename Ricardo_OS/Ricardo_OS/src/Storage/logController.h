#ifndef LOG_CONTROLLER_H
#define LOG_CONTROLLER_H
#include <Arduino.h>
#include "Sensors/sensorStructs.h"
#include "logframe.h"

#include "storageController.h"
#include <vector>
#include <string>
#include "flags.h"
#include <array>


#include "Loggers/logger.h"
#include "Loggers/telemetryLogger.h"
#include "Loggers/systemLogger.h"

//temporary for debugging
#include <rnp_networkmanager.h>

enum class LOG_TYPE:uint8_t{
    TELEMETRY = 0,
    SYSTEM = 1,
    NETWORK = 2,
    ALL
};

class LogController{

    public:
        LogController(StorageController* storagecontroller,RnpNetworkManager& netman);
        void setup();
        //telemtry logging
        void log(const SensorStructs::state_t &estimator_state ,const SensorStructs::raw_measurements_t &raw_sensors,bool force=false);
        //network logging
		//void log(PacketHeader &header);
        //system logging
        void log(const std::string &message);
        void log(uint32_t status,uint32_t flag,std::string message);
        void log(uint32_t status,uint32_t flag);

        void stopLogging(LOG_TYPE log);
        void startLogging(LOG_TYPE log);

        void generateLogDirectories(STORAGE_DEVICE device);

        void telemetryFrequency(uint16_t time_period);

        void update();

 

    private:

        StorageController* _storagecontroller; //pointer to storage controller
        RnpNetworkManager& _netman;

        SystemLogger systemlogger;
        uint16_t systemlogger_writeDelta = 1000; // in millis
        uint64_t systemlogger_prevTime = 0;

        TelemetryLogger telemetrylogger;
        uint16_t telemetrylogger_logDelta = 50; //in millis
        uint64_t telemetrylogger_prevTime = 0;


        //loging directory prefix;
        const std::string parentDirectory = "/Logs" ;
        std::string uniqueDirectory;

};

#endif
