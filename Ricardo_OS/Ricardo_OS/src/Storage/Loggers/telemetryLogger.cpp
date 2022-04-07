#include "telemetryLogger.h"

#include "logger.h"
#include <SdFat.h>
#include "config.h"
#include "../storageController.h"
#include <string>
#include "Sensors/sensorStructs.h"
#include "../logframe.h"

#include "Arduino.h"


TelemetryLogger::TelemetryLogger(StorageController* sc,std::string filename,STORAGE_DEVICE mainStorage,STORAGE_DEVICE backupStorage):
Logger(sc,filename,mainStorage,backupStorage)
{
telemetry_log_buffer.reserve(2248); // 1024 + 200 bytes 
};

void TelemetryLogger::enable(){
    Logger::enable(); // calling base here but option to override this
};


void TelemetryLogger::disable(){
    flush(); // ensure all frames are written  before disabling
    Logger::disable();
};


void TelemetryLogger::log(const SensorStructs::state_t &estimator_state ,const SensorStructs::raw_measurements_t &raw_sensors,uint64_t time){

    if (!_status){
        return; // check if logger is enabled
    }

    
    telemetry_frame.gps_long = raw_sensors.gps.lng;
    telemetry_frame.gps_lat = raw_sensors.gps.lat;
    telemetry_frame.gps_alt = raw_sensors.gps.alt;
    telemetry_frame.gps_v_n = raw_sensors.gps.v_n;
    telemetry_frame.gps_v_e = raw_sensors.gps.v_e;
    telemetry_frame.gps_v_d = raw_sensors.gps.v_d;
    telemetry_frame.gps_sat = raw_sensors.gps.sat;
    telemetry_frame.gps_fix = raw_sensors.gps.fix;
    telemetry_frame.ax = raw_sensors.accelgyro.ax;
    telemetry_frame.ay = raw_sensors.accelgyro.ay;
    telemetry_frame.az = raw_sensors.accelgyro.az;
    telemetry_frame.gx = raw_sensors.accelgyro.gx;
    telemetry_frame.gy = raw_sensors.accelgyro.gy;
    telemetry_frame.gz = raw_sensors.accelgyro.gz;
    telemetry_frame.mx = raw_sensors.mag.mx;
    telemetry_frame.my = raw_sensors.mag.my;
    telemetry_frame.mz = raw_sensors.mag.mz;
    telemetry_frame.imu_temp = raw_sensors.accelgyro.temp;
    telemetry_frame.baro_alt = raw_sensors.baro.alt;
    telemetry_frame.baro_temp = raw_sensors.baro.temp;
    telemetry_frame.baro_press = raw_sensors.baro.press;
    telemetry_frame.batt_volt = raw_sensors.batt.volt;
    telemetry_frame.batt_percent = raw_sensors.batt.percent;
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
    telemetry_frame.timestamp = time;

    //check capacity of vector
    telemetry_log_buffer += telemetry_frame.stringify();

    if (telemetry_log_buffer.size()>= 4096){
        flush();
    }

       
};
 
TelemetryLogger::~TelemetryLogger(){}

void TelemetryLogger::flush() 
{
    if (telemetry_log_buffer.size() == 0){
        return;
    }

    //we want to clear the log buffer even if the write fails as its "okay" to loose some of the telemetry data
    main_logfile.write(telemetry_log_buffer.c_str(),telemetry_log_buffer.size());
    backup_logfile.write(telemetry_log_buffer.c_str(),telemetry_log_buffer.size());

    
    main_logfile.flush();
    backup_logfile.flush();

    telemetry_log_buffer.clear();
};

