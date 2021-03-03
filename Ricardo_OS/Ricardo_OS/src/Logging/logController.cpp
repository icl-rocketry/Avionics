#include "logController.h"
#include "config.h"
#include "storageController.h"
#include "flags.h"
#include <string>
#include <Arduino.h>


LogController::LogController(StorageController* storagecontroller):
_storagecontroller(storagecontroller)
{
    //telemetry_log_buffer.reserve(255); // reserving 255 character buffer
    //system_log_buffer.reserve(255); // reserving 255 character buffer
    //network_log_buffer.reserve(255); // reserving 255 character buffer
    //engine_log_buffer.reserve(255); // reserving 255 character buffer
    
    
};

void LogController::log(state_t &estimator_state) {

}

void LogController::log(raw_measurements_t &raw_sensors) {
	
}

void LogController::log(PacketHeader &header) {
	
}

void LogController::log(std::string message) {
    system_log_buffer += "[Message]" + message + "\n";
}

void LogController::log(uint32_t status,uint32_t flag,std::string message) {
    //will create a new log frame each time it is called
    //update system_frame with new values
    system_frame.systemStatus = status;
    system_frame.systemFlag = flag;
    system_frame.message = message;
    std::string test = flagLevel(flag) + system_frame.stringify() + "\n";
    Serial.println(test.c_str());

    system_log_buffer += flagLevel(flag) + system_frame.stringify() + "\n"; //append string log buffer with new values and add new line character

	
}
void LogController::log(uint32_t status,uint32_t flag) {
    //will create a new log frame each time it is called
    system_frame.systemStatus = status;
    system_frame.systemFlag = flag;
    system_frame.message = "";
    std::string test = flagLevel(flag) + system_frame.stringify() + "\n";
    Serial.println(test.c_str());
 
    system_log_buffer += flagLevel(flag) + system_frame.stringify() + "\n"; //append string log buffer with new values and add new line character

}

void LogController::update(){
    uint64_t dt = millis() - prev_time;
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
    std::string log_file_path;
    std::string log_buffer;
    switch(log_type){
        case LOG_TYPE::TELEMETRY:
        {

            break;
        }
        case LOG_TYPE::SYSTEM:
        {
            log_file_path = "Logs/system_log.txt";
            log_buffer = system_log_buffer;
            Serial.println(log_buffer.c_str());
            _storagecontroller->write(log_file_path,log_buffer,STORAGE_DEVICE::MICROSD);
            system_log_buffer = "";
            break;
        }
        case LOG_TYPE::NETWORK:
        {
            break;
        }
    }
    //duplicate logs to all avaliable storage - > this might be really slow idk?
    //_storagecontroller->write(log_file_path,data,STORAGE_DEVICE::ALL);
    //for dev only going to write to microsd -> easier to tell if we got it right lol
    

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