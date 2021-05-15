#include "logController.h"
#include "config.h"
#include "storageController.h"
#include "flags.h"
#include <string>
#include <Arduino.h>
#include <vector>
#include <SdFat.h>


LogController::LogController(StorageController* storagecontroller):
_storagecontroller(storagecontroller),
systemlogger(storagecontroller,5000,"/system_log.txt",STORAGE_DEVICE::MICROSD,STORAGE_DEVICE::NONE), // overriding defaults to no backup device for debugging currently
telemetrylogger(storagecontroller,5000,"/telemetry_log.txt",STORAGE_DEVICE::MICROSD,STORAGE_DEVICE::NONE)
{};

void LogController::setup(){
    
    //generate a unique directory name wrt MICROSD
    uniqueDirectory = _storagecontroller->getUniqueDirectory(parentDirectory,STORAGE_DEVICE::MICROSD); // get updated directory prefix
    //check on flash if directory already exsists -> this can happen if the sd is wiped but not flash
    //if this is true generate unique directory wrt FLASH
    if (_storagecontroller->mkdir(uniqueDirectory,STORAGE_DEVICE::FLASH)){
        uniqueDirectory = _storagecontroller->getUniqueDirectory(parentDirectory,STORAGE_DEVICE::FLASH); // get updated directory prefix
    }

    //create new directory
    _storagecontroller->mkdir(uniqueDirectory,STORAGE_DEVICE::MICROSD);
    //_storagecontroller->mkdir(uniqueDirectory,STORAGE_DEVICE::FLASH);
    
    //update loggers with unique directory
    systemlogger.setDirectory(uniqueDirectory);
    telemetrylogger.setDirectory(uniqueDirectory);

    //enable loggers
    systemlogger.enable();
    telemetrylogger.enable();

}

void LogController::generateLogDirectories(STORAGE_DEVICE device){
    _storagecontroller->mkdir(uniqueDirectory,device);
}

void LogController::log(state_t &estimator_state,raw_measurements_t &raw_sensors,bool force) {
    if((millis()-telemetry_prev_log_time) > telemetry_log_frequency || force){
        telemetrylogger.log(estimator_state,raw_sensors);
        telemetry_prev_log_time = millis(); // update previous log time
    }
}


void LogController::log(PacketHeader &header) {}

void LogController::log(std::string message) {
    systemlogger.log(message);
}

void LogController::log(uint32_t status,uint32_t flag,std::string message) {
    systemlogger.log(status,flag,message);	
}

void LogController::log(uint32_t status,uint32_t flag) {
    systemlogger.log(status,flag,"flag logged");
}

void LogController::update(){

    systemlogger.writeLog();
    telemetrylogger.writeLog();


}


void LogController::change_write_Frequency(uint16_t time_period,LOG_TYPE log_type){
    //simple bounds checking
    switch(log_type){
        case LOG_TYPE::NETWORK:{
            break;
        }
        case LOG_TYPE::SYSTEM:{
            systemlogger.changeFrequency(time_period);
            break;
        }
        case LOG_TYPE::TELEMETRY:{
            telemetrylogger.changeFrequency(time_period);
            break;
        }
        default:{
            //do nothing
            break;
        }
    }
};

void LogController::telemetry_Frequency(uint16_t time_period){telemetry_log_frequency = time_period;};

void LogController::startLogging(LOG_TYPE log){
    switch(log){
        case LOG_TYPE::NETWORK:{
            break;
        }
        case LOG_TYPE::SYSTEM:{
            systemlogger.enable();
            break;
        }
        case LOG_TYPE::TELEMETRY:{
            telemetrylogger.enable();
            break;
        }
        case LOG_TYPE::ALL:{
            systemlogger.enable();
            telemetrylogger.enable();
            break;
        }
        default:{
            break;
        }
    }
}

void LogController::stopLogging(LOG_TYPE log){
    switch(log){
        case LOG_TYPE::NETWORK:{
            break;
        }
        case LOG_TYPE::SYSTEM:{
            systemlogger.disable();
            break;
        }
        case LOG_TYPE::TELEMETRY:{
            telemetrylogger.disable();
            break;
        }
        case LOG_TYPE::ALL:{
            systemlogger.disable();
            telemetrylogger.disable();
            break;
        }
        default:{
            break;
        }
    }
}