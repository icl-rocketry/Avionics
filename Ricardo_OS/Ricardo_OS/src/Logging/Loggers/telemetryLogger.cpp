#include "telemetryLogger.h"

#include "logger.h"
#include <SdFat.h>
#include "config.h"
#include "../storageController.h"
#include <string>
#include "Sensors/sensorStructs.h"
#include "../logframe.h"


TelemetryLogger::TelemetryLogger(StorageController* sc,uint16_t dt,std::string filename,STORAGE_DEVICE mainStorage,STORAGE_DEVICE backupStorage):
Logger(sc,dt,filename,mainStorage,backupStorage)
{
telemetry_log_buffer.reserve(25);//this we can work out as the telemtry logging rates are defined
};

void TelemetryLogger::enable(){
    Logger::enable(); // calling base here but option to override this
};


void TelemetryLogger::disable(){
    Logger::disable();
};

void TelemetryLogger::writeLog(){
    if (!_status){
        return; // check if logger is enabled
    }
    if (millis()-_prevWriteTime > _writeDelta){
        for (int i = 0; i< telemetry_log_buffer.size();i++){
            //processing each frame individually so we dont accidentally use all of heap
            std::string entry = telemetry_log_buffer[i].stringify();
            
            //microsd_file.print(entry.c_str());
            //microsd_file.write(entry.c_str(),entry.length());
            main_logfile.write(entry.c_str(),entry.length());
            //_storagecontroller->write(flash_file_path,entry,STORAGE_DEVICE::FLASH);
            
        }

        
        main_logfile.flush();

        _prevWriteTime = millis();

    }
};



void TelemetryLogger::log(state_t &estimator_state,raw_measurements_t &raw_sensors){

    if (!_status){
        return; // check if logger is enabled
    }

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
    //std::vector<uint8_t> data = telemetry_frame.serialize();
    //std::string data = telemetry_frame.stringify();
    //telemetry_logfile.write(data.data(),data.size());
       
};

TelemetryLogger::~TelemetryLogger(){};