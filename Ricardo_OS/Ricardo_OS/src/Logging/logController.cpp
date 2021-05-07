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
    
    //generate a unique directory name
    uniqueDirectory = _storagecontroller->getUniqueDirectory(parentDirectory,STORAGE_DEVICE::MICROSD); // get updated directory prefix
    
    //create new directory
    _storagecontroller->mkdir(uniqueDirectory,STORAGE_DEVICE::MICROSD);
    //_storagecontroller->mkdir(uniqueDirectory,STORAGE_DEVICE::MICROSD);

    //update loggers with unique directory
    systemlogger.setDirectory(uniqueDirectory);
    telemetrylogger.setDirectory(uniqueDirectory);

    //enable loggers
    systemlogger.enable();
    telemetrylogger.enable();

}


void LogController::log(state_t &estimator_state,raw_measurements_t &raw_sensors,bool force) {
    if((millis()-telemetry_prev_log_time) > telemetry_log_frequency || force){
        telemetrylogger.log(estimator_state,raw_sensors);
        telemetry_prev_log_time = millis(); // update previous log time
    }
}


void LogController::log(PacketHeader &header) {


	
}

void LogController::log(std::string message) {
  
}

void LogController::log(uint32_t status,uint32_t flag,std::string message) {

	
}
void LogController::log(uint32_t status,uint32_t flag) {

}

void LogController::update(){

    
    //could be made neater if using for loop however this would make readabilty worse
    
    if ((millis() - prev_write_time[(uint8_t)LOG_TYPE::TELEMETRY]) > write_frequency[(uint8_t)LOG_TYPE::TELEMETRY]){
        write_to_file(LOG_TYPE::TELEMETRY);
        //Serial.println(telemetry_log_buffer.size());
        ///telemetry_log_buffer.clear();
        prev_write_time[(uint8_t)LOG_TYPE::TELEMETRY] = millis(); // update previous time
    }
    
    if ((millis() - prev_write_time[(uint8_t)LOG_TYPE::SYSTEM]) > write_frequency[(uint8_t)LOG_TYPE::SYSTEM]){
        write_to_file(LOG_TYPE::SYSTEM);
        prev_write_time[(uint8_t)LOG_TYPE::SYSTEM] = millis(); // update previous time
    }

    if ((millis() - prev_write_time[(uint8_t)LOG_TYPE::NETWORK]) > write_frequency[(uint8_t)LOG_TYPE::NETWORK]){
        //write_to_file(LOG_TYPE::NETWORK);
        prev_write_time[(uint8_t)LOG_TYPE::NETWORK] = millis(); // update previous time
    }


}

void LogController::write_to_file(LOG_TYPE log_type){
    std::string microsd_file_path;
    std::string flash_file_path;
    
    
    switch(log_type){
        case LOG_TYPE::TELEMETRY:
        {

            //microsd_file_path = microsd_prefix + telemetry_log_filename;
            //flash_file_path = flash_prefix + system_log_filename;

            //File microsd_file = _storagecontroller->open(microsd_file_path,STORAGE_DEVICE::MICROSD,(O_WRITE | O_CREAT | O_AT_END));

            //if (!microsd_file){
            //if (!telemetry_logfile){
             //   return; // file is invalid
            //}
            
            for (int i = 0; i< telemetry_log_buffer.size();i++){
                //processing each frame individually so we dont accidentally use all of heap
                std::string entry = telemetry_log_buffer[i].stringify();
                
                //microsd_file.print(entry.c_str());
                //microsd_file.write(entry.c_str(),entry.length());
                telemetry_logfile.write(entry.c_str(),entry.length());
                //_storagecontroller->write(flash_file_path,entry,STORAGE_DEVICE::FLASH);
                
            }
            
            //microsd_file.close();
            telemetry_logfile.flush();
            //telemetry_log_buffer.clear(); //clear all log frames in buffer

            break;
        }
        case LOG_TYPE::SYSTEM:
        {
            microsd_file_path = microsd_prefix + system_log_filename;
            flash_file_path = flash_prefix + system_log_filename;

            File microsd_file = _storagecontroller->open(microsd_file_path,STORAGE_DEVICE::MICROSD,(O_WRITE | O_CREAT | O_AT_END));

            if (!microsd_file){
                return; // file is invalid
            }
            
            for (int i = 0; i< system_log_buffer.size();i++){
                //processing each frame individually so we dont accidentally use all of heap
                std::string entry = system_log_buffer[i].stringify();
                microsd_file.print(entry.c_str());
                //_storagecontroller->write(flash_file_path,entry,STORAGE_DEVICE::FLASH);
                
            }
            
            microsd_file.close();

            system_log_buffer.clear(); //clear all log frames in buffer

            break;
        }
        case LOG_TYPE::NETWORK:
        {
            break;
        }
    }
    

}




void LogController::change_write_Frequency(uint16_t time_period,LOG_TYPE log_type){
    //simple bounds checking
    if((uint8_t)log_type < write_frequency.size()){
        //update logging frequnecy
        write_frequency[(uint8_t)log_type] = time_period;
    }
};

void LogController::telemetry_Frequency(uint16_t time_period){telemetry_log_frequency = time_period;};