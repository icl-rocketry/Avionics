#include "logController.h"
#include "config.h"
#include "storageController.h"
#include "flags.h"
#include <string>
#include <Arduino.h>
#include <vector>
#include <SdFat.h>


LogController::LogController(StorageController* storagecontroller):
_storagecontroller(storagecontroller)

{
    telemetry_log_buffer.reserve(25); //this we can work out as the telemtry logging rates are defined
    system_log_buffer.reserve(10); //this we need to estimate
    network_log_buffer.reserve(10); //this needs to be estimated too
    
};

void LogController::setup(){
    
    
    microsd_prefix = _storagecontroller->updateDirectoryName(microsd_prefix,STORAGE_DEVICE::MICROSD); // get updated directory prefix
    
    //flash_prefix = _storagecontroller->updateDirectoryName(flash_prefix,STORAGE_DEVICE::FLASH); // get updated directory prefix
    //ensure directory exists
    //Serial.println(microsd_prefix.c_str());
    _storagecontroller->mkdir(microsd_prefix,STORAGE_DEVICE::MICROSD);
   // Serial.println(_storagecontroller->updateDirectoryName(old,STORAGE_DEVICE::MICROSD).c_str());
    //_storagecontroller->mkdir(flash_prefix,STORAGE_DEVICE::FLASH);
}


void LogController::log(state_t &estimator_state,raw_measurements_t &raw_sensors,bool force) {
 
    if((millis()-telemetry_prev_log_time) > telemetry_log_frequency || force){

        telemetry_frame.rawGPSLong = raw_sensors.gps_long; //continue for all variables - we need to see if thers a better way to do this
        telemetry_frame.rawGPSLat = raw_sensors.gps_lat;
        telemetry_frame.rawGPSsat = raw_sensors.gps_sat;
        telemetry_frame.rawIMUAx= raw_sensors.ax;
        telemetry_frame.rawIMUAy= raw_sensors.ay;
        telemetry_frame.rawIMUAz= raw_sensors.az;
        telemetry_frame.rawIMUGx= raw_sensors.gx;
        telemetry_frame.rawIMUGy= raw_sensors.gy;
        telemetry_frame.rawIMUGz= raw_sensors.gz;
        telemetry_frame.rawIMUMx= raw_sensors.mx;
        telemetry_frame.rawIMUMy= raw_sensors.my;
        telemetry_frame.rawIMUMz= raw_sensors.mz;
        telemetry_frame.rawIMUTemp= raw_sensors.imu_temp;
        telemetry_frame.rawTimestamp = millis();

        telemetry_log_buffer.push_back(telemetry_frame); // add frame to buffer

        telemetry_prev_log_time = millis(); // update previous log time
    }
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

            microsd_file_path = microsd_prefix + telemetry_log_filename;
            //flash_file_path = flash_prefix + system_log_filename;

            File microsd_file = _storagecontroller->open(microsd_file_path,STORAGE_DEVICE::MICROSD,(O_WRITE | O_CREAT | O_AT_END));

            if (!microsd_file){
                return; // file is invalid
            }
            
            for (int i = 0; i< telemetry_log_buffer.size();i++){
                //processing each frame individually so we dont accidentally use all of heap
                std::string entry = telemetry_log_buffer[i].stringify();
                
                microsd_file.print(entry.c_str());
                //_storagecontroller->write(flash_file_path,entry,STORAGE_DEVICE::FLASH);
                
            }
            
            microsd_file.close();

            telemetry_log_buffer.clear(); //clear all log frames in buffer

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

void LogController::change_write_Frequency(uint16_t time_period,LOG_TYPE log_type){
    //simple bounds checking
    if((uint8_t)log_type < write_frequency.size()){
        //update logging frequnecy
        write_frequency[(uint8_t)log_type] = time_period;
    }
};

void LogController::telemetry_Frequency(uint16_t time_period){telemetry_log_frequency = time_period;};