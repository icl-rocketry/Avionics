#include "baro.h"
#include "config.h"
#include "SPI.h"

#include "Logging/systemstatus.h"

#include "sensorStructs.h"


Baro::Baro(SPIClass* spi,SystemStatus* systemstatus,raw_measurements_t* raw_data):
_spi(spi),
_systemstatus(systemstatus),
_raw_data(raw_data)
{};

void Baro::setup(){
    //some sort of gps intilization idk
    
}

void Baro::update(){
    
    if (_spi != NULL){ //check for null pointer to rpevent undfiend behaviour
  
    };
}
