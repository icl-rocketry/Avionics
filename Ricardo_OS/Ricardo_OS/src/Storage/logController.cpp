#include "logController.h"
#include "config.h"
#include "storageController.h"
#include "flags.h"
#include <string>
#include <Arduino.h>
#include <vector>
#include <SdFat.h>

#include <rnp_networkmanager.h>
#include <rnp_packet.h>

LogController::LogController(StorageController* storagecontroller,RnpNetworkManager& netman):
_storagecontroller(storagecontroller),
_netman(netman),
systemlogger(storagecontroller,"/system_log.txt",STORAGE_DEVICE::MICROSD,STORAGE_DEVICE::NONE), // overriding defaults to no backup device for debugging currently
telemetrylogger(storagecontroller,"/telemetry_log.txt",STORAGE_DEVICE::MICROSD,STORAGE_DEVICE::NONE)
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
    _storagecontroller->mkdir(uniqueDirectory,STORAGE_DEVICE::FLASH);
    
    //update loggers with unique directory
    systemlogger.setDirectory(uniqueDirectory);
    telemetrylogger.setDirectory(uniqueDirectory);

    //enable loggers
    systemlogger.enable();
    //telmetry logging be default for debugging
    #ifdef DEBUG_ENABLED
    telemetrylogger.enable();
    #endif

}


void LogController::generateLogDirectories(STORAGE_DEVICE device){
    _storagecontroller->mkdir(uniqueDirectory,device);
}

void LogController::log(const SensorStructs::state_t &estimator_state ,const SensorStructs::raw_measurements_t &raw_sensors,bool force) {
    uint64_t current_time = micros();
    if((current_time-telemetrylogger_prevTime) >= (telemetrylogger_logDelta*1000) || force){
        telemetrylogger_prevTime = current_time; // update previous log time
        telemetrylogger.log(estimator_state,raw_sensors,current_time);
        
    }
}


void LogController::log(const std::string &message) {
    systemlogger.log(message);

    MessagePacket_Base<0,100> msg(message);
	msg.header.source = _netman.getAddress();
	msg.header.destination = 1;
	msg.header.uid = 0;
	_netman.sendPacket(msg);
    
}

void LogController::log(uint32_t status,uint32_t flag,std::string message) {
    systemlogger.log(status,flag,message);	

    MessagePacket_Base<0,100> msg("flag: " + std::to_string(flag) + " " + message);
	msg.header.source = _netman.getAddress();
	msg.header.destination = 1;
	msg.header.uid = 0;
	_netman.sendPacket(msg);
}

void LogController::log(uint32_t status,uint32_t flag) {
    systemlogger.log(status,flag,"flag logged");
}

void LogController::update(){
    //system logger is an event based log, but we dont want to write each time there is an event as this is unessecarily ineeficent
    //and we would like to if possible take advantage of multi-block writes so we call writelog periodically to check if we need to
    //write the logs to file. This means there is a possibilty that in the event of a crash (physical or computatinal) we may loose logs
    //so we must perform this check often enough.
    if (millis() - systemlogger_prevTime >= systemlogger_writeDelta){
        systemlogger_prevTime = millis();
        systemlogger.flush(); 
        
    }

}



void LogController::telemetryFrequency(uint16_t time_period){telemetrylogger_logDelta = time_period;};

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