#ifndef INTERNAL_IO_H
#define INTERNAL_IO_H
#include <Arduino.h>


//define global VSPI object
class SPIClass; //forward declaration

extern SPIClass vspi;

bool setup_io();

#endif