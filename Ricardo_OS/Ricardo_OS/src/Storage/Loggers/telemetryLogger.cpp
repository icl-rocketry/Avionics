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
telemetry_log_buffer.reserve(1224); // 1024 + 200 bytes 
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
    if (telemetry_log_buffer.size()>=1024){
        main_logfile.write(telemetry_log_buffer.c_str(),telemetry_log_buffer.size());
        backup_logfile.write(telemetry_log_buffer.c_str(),telemetry_log_buffer.size());

        
        main_logfile.flush();
        backup_logfile.flush();

        telemetry_log_buffer.clear();
    }

};



void TelemetryLogger::log(state_t &estimator_state,raw_measurements_t &raw_sensors){

    if (!_status){
        return; // check if logger is enabled
    }


    telemetry_frame.gps_long = raw_sensors.gps_long;
    telemetry_frame.gps_lat = raw_sensors.gps_lat;
    telemetry_frame.gps_alt = raw_sensors.gps_alt;
    telemetry_frame.gps_v_n = raw_sensors.gps_v_n;
    telemetry_frame.gps_v_e = raw_sensors.gps_v_e;
    telemetry_frame.gps_v_d = raw_sensors.gps_v_d;
    telemetry_frame.gps_sat = raw_sensors.gps_sat;
    telemetry_frame.gps_fix = raw_sensors.gps_fix;
    telemetry_frame.ax = raw_sensors.ax;
    telemetry_frame.ay = raw_sensors.ay;
    telemetry_frame.az = raw_sensors.az;
    telemetry_frame.gx = raw_sensors.gx;
    telemetry_frame.gy = raw_sensors.gy;
    telemetry_frame.gz = raw_sensors.gz;
    telemetry_frame.mx = raw_sensors.mx;
    telemetry_frame.my = raw_sensors.my;
    telemetry_frame.mz = raw_sensors.mz;
    telemetry_frame.imu_temp = raw_sensors.imu_temp;
    telemetry_frame.baro_alt = raw_sensors.baro_alt;
    telemetry_frame.baro_temp = raw_sensors.baro_temp;
    telemetry_frame.baro_press = raw_sensors.baro_press;
    telemetry_frame.batt_volt = raw_sensors.batt_volt;
    telemetry_frame.batt_percent = raw_sensors.batt_percent;
    telemetry_frame.roll = estimator_state.eulerAngles[0];
    telemetry_frame.pitch = estimator_state.eulerAngles[1];
    telemetry_frame.yaw = estimator_state.eulerAngles[2];
    telemetry_frame.pn = estimator_state.position[0];
    telemetry_frame.pe = estimator_state.position[1];
    telemetry_frame.pd = estimator_state.position[2];
    telemetry_frame.vn = estimator_state.velocity[0];
    telemetry_frame.ve = estimator_state.velocity[1];
    telemetry_frame.vd = estimator_state.velocity[2];
    telemetry_frame.an = estimator_state.acceleration[0];
    telemetry_frame.ae = estimator_state.acceleration[1];
    telemetry_frame.ad = estimator_state.acceleration[2];
    telemetry_frame.timestamp = millis();

    std::string string_data = telemetry_frame.stringify();

    //check capacity of vector
    if (telemetry_log_buffer.size() + string_data.size() > telemetry_log_buffer.capacity()){
        
        telemetry_log_buffer.clear(); // this should never happen but it prevents the logger eating all ram like the cookie monster
    }
    //telemetry_log_buffer.resize(telemetry_log_buffer.size() + string_data.size());
    telemetry_log_buffer += string_data;
    //memcpy(telemetry_log_buffer.data() + telemetry_log_buffer.size(),string_data.data(),string_data.size());
    
    //telemetry_log_buffer.push_back(telemetry_frame); // add frame to buffer
    //std::vector<uint8_t> data = telemetry_frame.serialize();
    //std::string data = telemetry_frame.stringify();
    //telemetry_logfile.write(data.data(),data.size());
       
};

TelemetryLogger::~TelemetryLogger(){};