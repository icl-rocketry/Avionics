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


enum class LOG_TYPE:uint8_t{
    TELEMETRY = 0,
    SYSTEM = 1,
    NETWORK = 2
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

        

        void change_write_Frequency(uint16_t time_period,LOG_TYPE log_type);

        void telemetry_Frequency(uint16_t time_period);
        
        //update function controls writing to file 
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

        //loging directory prefix;
        std::string microsd_prefix = "/Logs" ;
        std::string flash_prefix = "/Logs";
        //log file names
        const std::string telemetry_log_filename = "/telemetry_log.txt";
        const std::string system_log_filename = "/system_log.txt";
        const std::string network_log_filename = "/network_log.txt";

        //telemetry logging frequnecy variables
        uint16_t telemetry_log_frequency = 1000; // default frequnecy of 1Hz
        uint64_t telemetry_prev_log_time = 0;
        //writing to memory frequency
        std::array<uint16_t,3> write_frequency = {5000,5000,5000};
        std::array<uint64_t,3> prev_write_time = {0,0,0};
        //uint16_t log_frequency[3] = {5000,5000,5000};
    
        //methods to write buffer to file
        void write_to_file(LOG_TYPE log_type);
        
        //flag level helper function
        std::string flagLevel(uint32_t flag);
        std::string flagLevel(system_flag flag);


        

};

#endif
