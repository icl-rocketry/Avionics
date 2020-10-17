#include <Arduino.h>

#include "iface.h"

Iface::Iface(uint8_t address){
    _address = address; //assign address to interface
};

Iface::~Iface(){};