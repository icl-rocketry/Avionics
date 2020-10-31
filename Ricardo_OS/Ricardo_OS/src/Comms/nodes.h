#ifndef NODES_H
#define NODES_H
#include <Arduino.h>
//enum for id corresponding to node in network

enum class Nodes:uint8_t{
    ROCKET = 1,
    GROUNDSTATION = 2,
    DESKTOP = 3,
    MOTORBOARD = 4
};



#endif