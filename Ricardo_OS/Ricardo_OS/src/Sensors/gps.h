

#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
//#include "TinyGPS++.h" 
#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include "Wire.h"

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include "sensorStructs.h"


class GPS{
public:
    GPS(TwoWire* wire,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data);
    void setup();
    void update();

private:
    //TinyGPSPlus tinygps; //define tinygps object
    SFE_UBLOX_GNSS gnss;
    TwoWire *_wire; //pointer to wire object
    

    SystemStatus* _systemstatus;//pointer to system status object
    LogController* _logcontroller;//pointer to log controller

    raw_measurements_t*  _raw_data;//pointer to raw data struct
};

#endif
