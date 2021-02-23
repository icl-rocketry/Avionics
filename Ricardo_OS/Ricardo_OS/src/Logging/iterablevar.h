#ifndef ITERVAR_H
#define ITERVAR_H
//template class for an iterable member variable type

#include <string>
#include <vector>
#include "tostring.h"


template<typename T>
class iterVar{
    public:
        iterVar(T* var):_ptr(var)
        {};

        std::string stringify(){
            return tostring(&_ptr); //helper function to convert to string
        }
        std::vector<uint8_t> serialize(){//serialization of variable
            std::vector<uint8_t> bytearray(size);//intialize byte array
            memcpy(bytearray.data(), _ptr,size);//copy bytes from variable to byte array
            return bytearray
        }
        void serialize_to_buffer(std::vector<uint8_t> &buf){
            std::vector<uint8_t> bytearray = this->serialize();
            for (int i = 0; i<bytearray.size();i++){
                buf.push_back(bytearray[i]);
            }
        }
        void deserialize(const uint8_t* data){
            memcpy(_ptr,data,size);//copy size bytes from data buffer to pointer
        }
    private:
        T* _ptr;
        size_t size = sizeof(T);

}

// F for your deleted code

#endif