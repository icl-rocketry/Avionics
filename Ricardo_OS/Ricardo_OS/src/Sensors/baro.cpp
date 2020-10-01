#include "baro.h"
#include "config.h"
#include "SPI.h"



Baro::Baro(SPIClass* spi)
{
    _spi = spi;
};

void Baro::setup(){
    //some sort of gps intilization idk
    
}

void Baro::update(){
    
    if (_spi != NULL){ //check for null pointer to rpevent undfiend behaviour
  
    };
}
