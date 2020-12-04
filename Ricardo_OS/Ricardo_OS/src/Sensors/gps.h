

#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
#include "TinyGPS++.h" 
#include "Wire.h"

#include "Logging/systemstatus.h"

#include "sensorStructs.h"


class GPS{
public:
    GPS(TwoWire* wire,SystemStatus* systemstatus,raw_measurements_t* raw_data);
    void setup();
    void update();

private:
    TinyGPSPlus tinygps; //define tinygps object

    TwoWire *_wire; //pointer to wire object
      
    SystemStatus* _systemstatus;//pointer to system status object

    raw_measurements_t*  _raw_data;//pointer to raw data struct
};

#endif
