#include "baro.h"
#include "config.h"
#include "SPI.h"

#include "Logging/systemstatus.h"


Baro::Baro(SPIClass* spi,SystemStatus* systemstatus)
{
    _spi = spi;
    _systemstatus = systemstatus;
};

void Baro::setup(){
    //some sort of gps intilization idk
    
}

void Baro::update(){
    
    if (_spi != NULL){ //check for null pointer to rpevent undfiend behaviour
  
    };
}
