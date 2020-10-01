

#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
#include "TinyGPS++.h"//possible to remove these headers and make them forward declarations but cba unless we run into circular depndancies
#include "Wire.h"


struct gps_data_t{
    float lat,lng;
    //add more members if needed
};

class GPS{
    public:
        GPS(TwoWire* wire);
        void setup();
        void update();

        gps_data_t gps_data;

    private:
        //define tinygps object
        TinyGPSPlus tinygps;
        //pointer to wire object
        TwoWire* _wire;

        
};


#endif
