#include "Wire.h"
#include "SPI.h"
#include <Arduino.h>

#ifndef INTERNAL_IO_H
#define INTERNAL_IO_H

//define i2c object
TwoWire I2C = TwoWire(0);

//define VSPI object
SPIClass vspi = SPIClass(VSPI);

void setup_io();

#endif