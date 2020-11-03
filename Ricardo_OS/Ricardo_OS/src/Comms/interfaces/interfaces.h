#ifndef INTERFACES_H
#define INTERFACES_H
#include <Arduino.h>
//enum for id corresonding to interface

enum class Interface:uint8_t{
    LOOPBACK = 0, //currently just a nice thought and to make the routing table more readable can be used in the future to test interfaces 
    LORA = 1,
    USBSerial = 2,
    CAN = 3

};

#endif