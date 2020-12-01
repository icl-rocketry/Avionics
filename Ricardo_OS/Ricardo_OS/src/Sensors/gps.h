

#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
#include "TinyGPS++.h" 
#include "Wire.h"

#include "Logging/systemstatus.h"



struct gps_data_t
{
    float lat, lng, alt, course, speed, hdop, sats;
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
