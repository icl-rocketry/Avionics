#ifndef BARO_H
#define BARO_H
#include <Arduino.h>
#include "SPI.h"

#include "Logging/systemstatus.h"


struct baro_data_t{
    float temp,pressure;
    //add more members if needed
};

class Baro{
    public:
        Baro(SPIClass* spi,SystemStatus* systemstatus);
        void setup();
        void update();

        baro_data_t baro_data;

    private:
        //pointer to spi object
        SPIClass* _spi;
        //pointer to system status object
        SystemStatus* _systemstatus;


};    

#endif
