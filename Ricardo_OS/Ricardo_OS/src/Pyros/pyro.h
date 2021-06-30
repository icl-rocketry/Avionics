//abstract pyro interface

#ifndef PYRO_H
#define PYRO_H

#include <Arduino.h>
#include "Storage/systemstatus.h"


enum class PYRO_TYPE:uint8_t{
    LOCAL, //local gpio type igniter
    REMOTE_BT //bluetooth remote igniter
};

class Pyro{

    public:
        Pyro(PYRO_TYPE type,SystemStatus* systemstatus,uint8_t id);     
        /**
         * @brief Check continiuty of ematch
         * 
         * @return true 
         * @return false 
         */
        virtual bool check_continuity() = 0; 
        /**
         * @brief Fire the pyro
         * 
         * @param duration 
         */
        virtual void doStuff(uint8_t duration);
        /**
         * @brief Get the Continuity status
         * 
         * @return true 
         * @return false 
         */
        bool getContinuity(){return _continuity;};
        
        virtual ~Pyro() = 0;

    protected:
        SystemStatus* _systemstatus; // pointer to system status object
        PYRO_TYPE _type; //type of igniter
        bool _continuity; 
        uint8_t _id; // pyro id for logging purposes

       
};

#endif