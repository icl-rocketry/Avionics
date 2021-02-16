#ifndef LOG_CONTROLLER_H
#define LOG_CONTROLLER_H
#include <Arduino.h>
#include "Sensors/sensorStructs.h"
#include "logframe.h"
#include "Comms/packets.h"
#include "storageController.h"

class LogController{
    public:
        LogController(StorageController* storagecontroller);

        void log(state_t estimator_state);
        void log(raw_measurements_t raw_sensors);
		void log(PacketHeader header);

    private:
        StorageController* _storagecontroller; //pointer to storage controller

        telemetry_logframe telemetry_frame; //telemetry log frame object
        system_logframe system_frame;   //system log frame object
        network_logframe network_frame; //network log frame object
        

};

#endif
