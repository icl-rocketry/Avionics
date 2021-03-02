#ifndef LOG_CONTROLLER_H
#define LOG_CONTROLLER_H
#include <Arduino.h>
#include "Sensors/sensorStructs.h"
#include "logframe.h"
#include "Comms/packets.h"
#include "storageController.h"
#include <vector>

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

        void write_to_file();


    private:
        StorageController* _storagecontroller; //pointer to storage controller

        telemetry_logframe telemetry_frame; //current telemetry log frame object
        system_logframe system_frame;   //currentsystem log frame object
        network_logframe network_frame; //current network log frame object
        //log frame buffers to temporarily store log frames before writing to storage devices
        std::vector<telemetry_logframe> telemetry_frame_buffer;
        std::vector<system_logframe> system_frame_buffer;
        std::vector<network_logframe> network_frame_buffer;
        std::vector<network_logframe> engine_frame_buffer;
        


        

};

#endif
