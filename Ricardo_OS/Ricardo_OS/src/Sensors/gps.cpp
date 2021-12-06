#include "gps.h"

#include "config.h"
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
//#include "TinyGPS++.h"
#include "Wire.h"


#include "Storage/systemstatus.h"
#include "Storage/logController.h"


#include "sensorStructs.h"
#include "flags.h"

// GPS: U-Blox Max-M8
// SDA pin 16; SCL pin 17


// http://arduiniana.org/libraries/tinygpsplus/
// written by Luis Marques



GPS::GPS(TwoWire* wire, SystemStatus* systemstatus, LogController* logcontroller,raw_measurements_t* raw_data) :
    gnss(),
    _wire(wire),
    _systemstatus(systemstatus),
    _logcontroller(logcontroller),
    _raw_data(raw_data)
{}

void GPS::setup()
{
    //_wire->begin();
    if(!gnss.begin(*_wire)){
        _systemstatus->new_message(SYSTEM_FLAG::ERROR_GPS,"GPS I2C not found at address");
        _i2cerror = true;
    }else{
        _logcontroller->log("GPS Initialized");   
        _i2cerror = false;
    }
    //turn off nmea messaging
    gnss.setI2COutput(COM_TYPE_UBX);
    if (!gnss.setDynamicModel(DYN_MODEL_AIRBORNE4g)){
        _logcontroller->log("GPS failed to set dynamics model");
    } 
    gnss.setNavigationFrequency(10); //Set output to 10 times a second
    gnss.setAutoPVT(true);

}

void GPS::update()
{
    if (_i2cerror){
        return;
    }

   if (gnss.getPVT() && (!gnss.getInvalidLlh())){ // check if new navigation solution is available
        _raw_data->gps_updated = true;

       _raw_data->gps_lat = (float)gnss.getLatitude()/10000000.0; //degrees
       _raw_data->gps_long = (float)gnss.getLongitude()/10000000.0; // degrees
       _raw_data->gps_alt = gnss.getAltitude(); //mm above ellipsoid

       _raw_data->gps_pdop = gnss.getPDOP();

       _raw_data->gps_v_n = gnss.getNedNorthVel(); // mm/s
       _raw_data->gps_v_e = gnss.getNedEastVel(); // mm/s
       _raw_data->gps_v_d = gnss.getNedDownVel(); // mm/s

       _raw_data->gps_sat = gnss.getSIV(); 
       _raw_data->gps_fix = gnss.getFixType();
       _raw_data->gps_valid = gnss.getGnssFixOk();

       _raw_data->gps_year = gnss.getYear();
       _raw_data->gps_month = gnss.getMonth();
       _raw_data->gps_day = gnss.getDay();
       _raw_data->gps_hour = gnss.getHour();
       _raw_data->gps_minute = gnss.getMinute();
       _raw_data->gps_second = gnss.getSecond();



    //    gnss.flushPVT();
   }else{
       _raw_data->gps_updated = false;
   }
    
}