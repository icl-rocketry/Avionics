#include "systemLogger.h"

#include "logger.h"
#include <SdFat.h>
#include "config.h"
#include "../storageController.h"
#include <string>
#include "Sensors/sensorStructs.h"
#include "../logframe.h"


SystemLogger::SystemLogger(StorageController* sc,uint16_t dt,std::string filename,STORAGE_DEVICE mainStorage,STORAGE_DEVICE backupStorage):
Logger(sc,dt,filename,mainStorage,backupStorage)
{
    system_log_buffer.reserve(10); //this we need to estimate
};
SystemLogger::~SystemLogger(){};

void SystemLogger::enable(){
    Logger::enable(); // calling base here but option to override this
};

void SystemLogger::disable(){
    Logger::disable();
};

void SystemLogger::writeLog(){
    if (!_status){
        return; // check if logger is enabled
    }
    if (millis()-_prevWriteTime > _writeDelta){
        for (int i = 0; i< system_log_buffer.size();i++){
            //processing each frame individually so we dont accidentally use all of heap
            std::string entry = system_log_buffer[i].stringify();
            
            //microsd_file.print(entry.c_str());
            //microsd_file.write(entry.c_str(),entry.length());
            main_logfile.write(entry.c_str(),entry.length());
            //_storagecontroller->write(flash_file_path,entry,STORAGE_DEVICE::FLASH);
            
        }
       
        
        main_logfile.flush();
        system_log_buffer.clear();

        _prevWriteTime = millis();

    }
};


void SystemLogger::log(std::string message){
    system_frame.logLevel = "[MESSAGE]";
    system_frame.timestamp = millis();
    system_frame.systemStatus = 0;
    system_frame.systemFlag = 0;
    system_frame.message = message;

 
    system_log_buffer.push_back(system_frame);
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
    system_log_buffer.push_back(system_frame);
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
};