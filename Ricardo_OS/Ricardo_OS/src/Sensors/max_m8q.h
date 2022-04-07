#pragma once

#include <SparkFun_u-blox_GNSS_Arduino_Library.h>
#include <Wire.h>

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

#include "sensorStructs.h"


#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>


class Max_M8Q{
    public:
        Max_M8Q(TwoWire& wire,SystemStatus& systemstatus,LogController& logcontroller);
        void setup();
        void update(SensorStructs::GPS_t& gpsdata);

    private:
        SFE_UBLOX_GNSS gnss;
        TwoWire& _wire; //pointer to wire object
        
        SystemStatus& _systemstatus;//pointer to system status object
        LogController& _logcontroller;//pointer to log controller


        bool _i2cerror;// true if i2c failed to start
};

