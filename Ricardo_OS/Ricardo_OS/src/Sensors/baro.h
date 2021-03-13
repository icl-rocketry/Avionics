#ifndef BARO_H
#define BARO_H
#include <Arduino.h>
#include "SPI.h"

#include "Logging/systemstatus.h"
#include "Logging/logController.h"
#include "sensorStructs.h"

class Baro{
    public:
        Baro(SPIClass* spi,SystemStatus* systemstatus,LogController* logcontroller,raw_measurements_t* raw_data);
        void setup();
        void update();

    private:
        //pointer to spi object
        SPIClass* _spi;
        //pointer to system status object
        SystemStatus* _systemstatus;
        LogController* _logcontroller;
        //pointer to raw data struct
        raw_measurements_t* _raw_data;


};    

#endif
