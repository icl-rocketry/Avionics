#ifndef LOG_CONTROLLER_H
#define LOG_CONTROLLER_H
#include <Arduino.h>
#include "Sensors/sensorStructs.h"
#include "logframe.h"
#include "Comms/packets.h"
#include "storageController.h"
#include <vector>
#include <string>
#include "flags.h"
#include <array>


#include "Loggers/logger.h"
#include "Loggers/telemetryLogger.h"
#include "Loggers/systemLogger.h"



enum class LOG_TYPE:uint8_t{
    TELEMETRY = 0,
    SYSTEM = 1,
    NETWORK = 2,
    ALL
};

class LogController{

    public:
        LogController(StorageController* storagecontroller);
        void setup();
        //telemtry logging
        void log(state_t &estimator_state ,raw_measurements_t &raw_sensors,bool force=false);
        //network logging
		void log(PacketHeader &header);
        //system logging
        void log(std::string message);
        void log(uint32_t status,uint32_t flag,std::string message);
        void log(uint32_t status,uint32_t flag);

        void stopLogging(LOG_TYPE log);
        void startLogging(LOG_TYPE log);

        void generateLogDirectories(STORAGE_DEVICE device);
        

        void change_write_Frequency(uint16_t time_period,LOG_TYPE log_type);

        void telemetry_Frequency(uint16_t time_period);
        
        //update function controls writing to file 
        void update();

 
        

    private:

        StorageController* _storagecontroller; //pointer to storage controller

        SystemLogger systemlogger;
        TelemetryLogger telemetrylogger;

        //loging directory prefix;
        const std::string parentDirectory = "/Logs" ;
        std::string uniqueDirectory;

        //telemetry logging frequnecy variables
        uint16_t telemetry_log_frequency = 50; 
        uint64_t telemetry_prev_log_time = 0;

        

};

#endif
