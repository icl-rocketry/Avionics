#ifndef ITERVAR_H
#define ITERVAR_H

#include <string>
#include <vector>


template<typename T>

class iterVar{
    public:
        iterVar(T* var):_ptr(var){};
        std::string stringify(){
            return tostring(&_ptr);
        }
    private:
        T* _ptr;
        size_t size = sizeof(T);

}

// F for your deleted code

#endif