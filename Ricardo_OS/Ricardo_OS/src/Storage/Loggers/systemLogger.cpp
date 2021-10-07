#include "systemLogger.h"

#include "logger.h"
#include <SdFat.h>
#include "config.h"
#include "../storageController.h"
#include <string>
#include "Sensors/sensorStructs.h"
#include "../logframe.h"


SystemLogger::SystemLogger(StorageController* sc,std::string filename,STORAGE_DEVICE mainStorage,STORAGE_DEVICE backupStorage):
Logger(sc,filename,mainStorage,backupStorage)
{};

SystemLogger::~SystemLogger(){};

void SystemLogger::enable(){
    Logger::enable(); // calling base here but option to override this
};

void SystemLogger::disable(){
    flush();
    Logger::disable();
};

void SystemLogger::flush(){
    if (!_status){
        return; // check if logger is enabled
    }

    if (system_log_buffer.size() == 0){
        return;
    }

    bool error = false;

    while(system_log_buffer.size() > 0){

        std::string entry = system_log_buffer.front().stringify(); //get string representation of the first element
        if (entry.size() + raw_data_buffer.size() <= raw_data_buffer_max_size){
            raw_data_buffer += entry;
        }

        if (raw_data_buffer.size() >= 2048){ // try to take advantage of multi block writes

            error = writeRawBuffer();
            
        }

        system_log_buffer.pop(); // remove front element from queue
        
    }

    if (raw_data_buffer.size() > 0 && !error){
        error = writeRawBuffer();
    }

    main_logfile.flush();
    backup_logfile.flush();

};


void SystemLogger::log(const std::string &message){
    system_frame.logLevel = "[MESSAGE]";
    system_frame.timestamp = millis();
    system_frame.systemStatus = 0;
    system_frame.systemFlag = 0;
    system_frame.message = message;

 
    system_log_buffer.push(system_frame);
};
void SystemLogger::log(uint32_t status,uint32_t flag,std::string message){
    //will create a new log frame each time it is called
    //update system_frame with new values
    //construct log frame
    system_frame.logLevel = flagLevel(flag);
    system_frame.timestamp = millis();
    system_frame.systemStatus = status;
    system_frame.systemFlag = flag;
    system_frame.message = message;
    

    //add frame to buffer
    system_log_buffer.push(system_frame);
};




std::string SystemLogger::flagLevel(uint32_t flag){
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

std::string SystemLogger::flagLevel(SYSTEM_FLAG flag){
    return flagLevel(static_cast<uint32_t>(flag));
}

bool SystemLogger::writeRawBuffer() 
{
    bool main_log_ok = main_logfile.write(raw_data_buffer.c_str(),raw_data_buffer.length());
    bool backup_log_ok = backup_logfile.write(raw_data_buffer.c_str(),raw_data_buffer.length());

    if (!(main_log_ok && backup_log_ok)){ 
        if ((main_logfile.getDevice() == STORAGE_DEVICE::NONE) || (backup_logfile.getDevice() == STORAGE_DEVICE::NONE)){ // the case where both devices are none will never be handled here as the write function will return true if a none device is set
            // if we have encountered errors writing in either log files and if either log file is not assigned a device
            //then we want to try to not loose the data unless we exceed max size of buffer
            return false; // indicate we are in error 
        }
                    
    }
    raw_data_buffer.clear();
    return true;
};