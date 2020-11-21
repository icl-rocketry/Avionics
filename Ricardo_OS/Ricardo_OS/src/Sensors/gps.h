

#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
#include "TinyGPS++.h" //possible to remove these headers and make them forward declarations but cba unless we run into circular depndancies
#include "Wire.h"

#include "Logging/systemstatus.h"



struct gps_data_t
{
    float lat, lng, alt, course, speed, hdop;
};


class GPS{
public:
    GPS(TwoWire* wire,SystemStatus* systemstatus);
    void setup();
    void update();

    gps_data_t gps_data;

private:
    TinyGPSPlus tinygps; //define tinygps object

    TwoWire *_wire; //pointer to wire object
      
    SystemStatus* _systemstatus;//pointer to system status object
};

#endif
