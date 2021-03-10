#include "logController.h"
#include "config.h"
#include "storageController.h"
#include "flags.h"
#include <string>
#include <Arduino.h>
#include <vector>


LogController::LogController(StorageController* storagecontroller):
_storagecontroller(storagecontroller)

{
    telemetry_log_buffer.reserve(10); 
    system_log_buffer.reserve(10); 
    network_log_buffer.reserve(10); 
    
};

void LogController::setup(){
    microsd_prefix = _storagecontroller->updateDirectoryName(microsd_prefix,STORAGE_DEVICE::MICROSD); // get updated directory prefix
    flash_prefix = _storagecontroller->updateDirectoryName(flash_prefix,STORAGE_DEVICE::FLASH); // get updated directory prefix
    //ensure directory exists
    _storagecontroller->mkdir(microsd_prefix,STORAGE_DEVICE::MICROSD);
    _storagecontroller->mkdir(flash_prefix,STORAGE_DEVICE::FLASH);
}

void LogController::log(state_t &estimator_state) {

}

void LogController::log(raw_measurements_t &raw_sensors) {
	
}

void LogController::log(PacketHeader &header) {


	
}

void LogController::log(std::string message) {
    system_frame.logLevel = "[MESSAGE]";
    system_frame.timestamp = millis();
    system_frame.systemStatus = 0;
    system_frame.systemFlag = 0;
    system_frame.message = message;

 
    system_log_buffer.push_back(system_frame);
}

void LogController::log(uint32_t status,uint32_t flag,std::string message) {
    //will create a new log frame each time it is called
    //update system_frame with new values
    //construct log frame
    system_frame.logLevel = flagLevel(flag);
    system_frame.timestamp = millis();
    system_frame.systemStatus = status;
    system_frame.systemFlag = flag;
    system_frame.message = message;
    

    //add frame to buffer
    system_log_buffer.push_back(system_frame);
    
	
}
void LogController::log(uint32_t status,uint32_t flag) {
    //will create a new log frame each time it is called
    system_frame.logLevel = flagLevel(flag);
    system_frame.timestamp = millis();
    system_frame.systemStatus = status;
    system_frame.systemFlag = flag;
    system_frame.message = "flag logged";

 
    system_log_buffer.push_back(system_frame);
    
}

void LogController::update(){
    uint64_t dt = millis() - prev_time;
    //temporary heart beat log to ensure logging is funtioning
    

    if (dt > log_frequency[(uint8_t)LOG_TYPE::TELEMETRY]){
        //write_to_file(LOG_TYPE::TELEMETRY);
        prev_time = millis(); // update previous time
    }
    if (dt > log_frequency[(uint8_t)LOG_TYPE::SYSTEM]){
        write_to_file(LOG_TYPE::SYSTEM);
        prev_time = millis(); // update previous time
    }
    if (dt > log_frequency[(uint8_t)LOG_TYPE::NETWORK]){
        //write_to_file(LOG_TYPE::NETWORK);
        prev_time = millis(); // update previous time
    }


}

void LogController::write_to_file(LOG_TYPE log_type){
    std::string microsd_file_path;
    std::string flash_file_path;
    
    
    switch(log_type){
        case LOG_TYPE::TELEMETRY:
        {

            break;
        }
        case LOG_TYPE::SYSTEM:
        {
            microsd_file_path = microsd_prefix + system_log_filename;
            flash_file_path = flash_prefix + system_log_filename;
            
            for (int i = 0; i< system_log_buffer.size();i++){
                //processing each frame individually so we dont accidentally use all of heap
                std::string entry = system_log_buffer[i].stringify();
                _storagecontroller->write(microsd_file_path,entry,STORAGE_DEVICE::MICROSD);
                //_storagecontroller->write(flash_file_path,entry,STORAGE_DEVICE::FLASH);
                
            }
            
            system_log_buffer.clear(); //clear all log frames in buffer

            break;
        }
        case LOG_TYPE::NETWORK:
        {
            break;
        }
    }
    

}


std::string LogController::flagLevel(uint32_t flag){
    //using definitions from flags.h
    if (flag <= (1<<6)){
        return "[STATE]";
    }
    else if (flag <= (1<<7)){
        return "[DEBUG]";
    }
    else if (flag <= (1<<20)){
        return "[ERROR]";
    }
    else if (flag <= (1<<23)){
        return "[WARN]";
    }
    else{
        return "";
    }

};

std::string LogController::flagLevel(system_flag flag){
    return flagLevel(static_cast<uint32_t>(flag));
};