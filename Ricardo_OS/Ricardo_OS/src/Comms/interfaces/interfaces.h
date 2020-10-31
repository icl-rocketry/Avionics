#ifndef INTERFACES_H
#define INTERFACES_H
#include <Arduino.h>
//enum for id corresonding to interface
enum class Interface:uint8_t{
    LORA = 1,
    USBSerial = 2,
    CAN = 3

}

#endif