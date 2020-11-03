#ifndef NODES_H
#define NODES_H
#include <Arduino.h>
//enum for id corresponding to node in network

enum class Nodes:uint8_t{
    ROCKET = 0,
    GROUNDSTATION = 1,
    DESKTOP = 2,
    MOTORBOARD = 3
};



#endif