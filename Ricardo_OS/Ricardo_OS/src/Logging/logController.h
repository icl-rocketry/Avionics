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


enum class LOG_TYPE:uint8_t{
    TELEMETRY = 0,
    SYSTEM = 1,
    NETWORK = 2
};

class LogController{
    public:
        LogController(StorageController* storagecontroller);

        //telemtry logging
        void log(state_t &estimator_state);
        void log(raw_measurements_t &raw_sensors);
        //network logging
		void log(PacketHeader &header);
        //system logging
        void log(std::string message);
        void log(uint32_t status,uint32_t flag,std::string message);
        void log(uint32_t status,uint32_t flag);

        //update the buffers
        void update();

        


    private:

        StorageController* _storagecontroller; //pointer to storage controller
        telemetry_logframe telemetry_frame; //current telemetry log frame object
        system_logframe system_frame;   //currentsystem log frame object
        network_logframe network_frame; //current network log frame object
        //log frame buffers to temporarily store log frames before writing to storage devices

        std::vector<telemetry_logframe> telemetry_log_buffer;
        std::vector<system_logframe> system_log_buffer;
        std::vector<network_logframe> network_log_buffer;

        //logging frequnecies
        uint16_t log_frequency[3] = {5000,5000,5000};
    
        //methods to write buffer to file
        void write_to_file(LOG_TYPE log_type);
        
        //inialize previous time at 0
        uint64_t prev_time = 0;
        
        //flag level helper function
        std::string flagLevel(uint32_t flag);
        std::string flagLevel(system_flag flag);


        

};

#endif
