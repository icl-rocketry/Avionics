#include "gps.h"

#include "config.h"
#include "TinyGPS++.h"
#include "Wire.h"


#include "Logging/systemstatus.h"

#include "sensorStructs.h"

// GPS: U-Blox Max-M8
// SDA pin 16; SCL pin 17


// http://arduiniana.org/libraries/tinygpsplus/
// written by Luis Marques



GPS::GPS(TwoWire* wire, SystemStatus* systemstatus,raw_measurements_t* raw_data) :
    tinygps(),
    _wire(wire),
    _systemstatus(systemstatus),
    _raw_data(raw_data)
{}

void GPS::setup()
{
    //_wire->begin();
    
}

void GPS::update()
{

    if (_wire != NULL)
    { //check for null pointer to rpevent undfiend behaviour
        
        // Retrieve number of available bytes of data
        uint16_t sizeDataStream;
        _wire->beginTransmission(I2C_GPS_ADDRESS);
        _wire->write(GPS_NUM_AVAILABLE_BYTES_REGISTER);
        _wire->endTransmission(false);          // false -> does not release bus
        _wire->requestFrom(I2C_GPS_ADDRESS, 2); // reads 0xFD and 0xFE, which store the number of bytes in the data stream

        // _wire->available() returns number of available bytes
        while (_wire->available() > 0)
        {
            sizeDataStream = _wire->read();
            sizeDataStream = sizeDataStream << 8;
            sizeDataStream |= _wire->read();
        }

        // Gets available data
        _wire->beginTransmission(I2C_GPS_ADDRESS);
        _wire->write(GPS_DATASTREAM_REGISTER);
        _wire->endTransmission(false);

        // in case we want to store this somewhere?
        uint8_t data[sizeDataStream] = {};
        uint8_t index = 0;


        while (sizeDataStream > 0)
        {
            // requestFrom() limited to 32bytes
            uint16_t maxBytes = 32;
            uint16_t requestedBytes = min(sizeDataStream, maxBytes);

            // reads data-stream
            _wire->requestFrom(I2C_GPS_ADDRESS, requestedBytes);

            for (int i = index; i < index + requestedBytes; i++)
            {
                data[i] = _wire->read();

                tinygps.encode(_wire->read()); // passing data to tinygps
            }

            index += requestedBytes;
            sizeDataStream -= requestedBytes;
        }

        // Updates class values
        if (tinygps.altitude.isUpdated())
        {
            _raw_data->gps_alt = tinygps.altitude.meters();
        }

        if (tinygps.location.isUpdated())
        {
            _raw_data->gps_lat = tinygps.location.lat();
            _raw_data->gps_long = tinygps.location.lng();
            //Serial.println(gps_data.lat);
            //Serial.println(gps_data.lng);
        }

        if (tinygps.course.isUpdated())
        {
            _raw_data->gps_course = tinygps.course.deg();
        }

        if (tinygps.speed.isUpdated())
        {
            _raw_data->gps_speed = tinygps.speed.mps();
        }

        if (tinygps.hdop.isUpdated())
        {
            _raw_data->gps_hdop = tinygps.hdop.value(); // Horizontal Dim. of Precision
        }
        if(tinygps.satellites.isUpdated())
        {
            _raw_data->gps_sat = tinygps.satellites.value();//number of connected satellites
        }

    };
}