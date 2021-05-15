#pragma once

//templated function to convert numeric value types to std::string

#include <string>
#include <Arduino.h>
#include <sstream>


namespace utils {

inline int intify(std::string value) {
    int ret;
    std::stringstream str(value);
    str >> ret;

    return ret;
}


template<typename T>
inline std::string tostring(T value){
    std::ostringstream stm;
    stm << value;
    return stm.str();
};


}

