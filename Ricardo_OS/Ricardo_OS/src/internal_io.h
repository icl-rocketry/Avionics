#ifndef INTERNAL_IO_H
#define INTERNAL_IO_H

#include "Wire.h"
#include "SPI.h"
#include <Arduino.h>


//define global VSPI object
extern SPIClass vspi;

bool setup_io();

#endif