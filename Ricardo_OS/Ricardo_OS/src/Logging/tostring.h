#ifndef TOSTRING_H
#define TOSTRING_H

//templated function to convert numeric value types to std::string

#include <string>
#include <Arduino.h>
#include <sstream>


template<typename T>
std::string tostring(T value){
    std::ostringstream stm;
    stm << value;
    return stm.str();
};

#endif