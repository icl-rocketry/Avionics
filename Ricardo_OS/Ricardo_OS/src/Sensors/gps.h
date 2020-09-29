

#ifndef GPS_H
#define GPS_H
#include <Arduino.h>
#include "TinyGPS++.h"
#include "Wire.h"


struct gps_data_t{
    float lat,lng;
    //add more members if needed
};

class GPS{
    public:
        GPS();
        void setup(TwoWire* wire);
        void update();

        gps_data_t gps_data;

    private:
        //define tinygps object
        TinyGPSPlus tinygps;
        //pointer to wire object
        TwoWire* _wire;

        
};


#endif
