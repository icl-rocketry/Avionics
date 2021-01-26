

#ifndef PYRO_H
#define PYRO_H

#include <Arduino.h>
#include "stateMachine.h"

enum class PYRO_TYPE:uint8_t{
    LOCAL, //local gpio type igniter
    REMOTE_BT //bluetooth remote igniter
};

class Pyro{

    public:
        Pyro(uint8_t continuity_pin,uint8_t fire_pin);
        Pyro(uint8_t* interface);
        
        
        void check_continuity(); // update status of continuity 
    
        void Fire(uint8_t duration);

    private:
        stateMachine* _sm; //pointer to statemachine
        PYRO_TYPE _type; //type of igniter
        bool _continuity; 


        
};

#endif