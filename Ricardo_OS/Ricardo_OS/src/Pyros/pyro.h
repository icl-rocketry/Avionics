#ifndef PYRO_H
#define PYRO_H

#include <Arduino.h>


enum class PYRO_TYPE:uint8_t{
    LOCAL, //local gpio type igniter
    REMOTE_BT //bluetooth remote igniter
};

class Pyro{

    public:
        Pyro(PYRO_TYPE type);     
        virtual bool check_continuity(); // update status of continuity 
    
        virtual bool Fire(uint8_t duration);
        
        virtual ~Pyro() = 0;

    protected:
        
        PYRO_TYPE _type; //type of igniter
        bool _continuity = false; 

       
};

#endif