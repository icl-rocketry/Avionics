#include "gps.h"
#include "config.h"
#include "TinyGPS++.h"
#include "Wire.h"




GPS::GPS() :
    tinygps()
{

}

void GPS::setup(TwoWire* wire){
    //some sort of gps intilization idk
    _wire = wire;
}

void GPS::update(){
    //maybe change this to get specifc values like a lat function instead of update
    //someone needs to write this out and parse data into tinygps object i think this code should open up the ddc port for data transmission but havent tested it
    _wire->beginTransmission(I2C_GPS_ADDRESS);
    _wire->write(0xFF);
    _wire->endTransmission();
    
}