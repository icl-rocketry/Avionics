//base pyro interface

#ifndef PYRO_H
#define PYRO_H

#include <Arduino.h>

#include "Storage/systemstatus.h"
#include "Storage/logController.h"

enum class PYROTYPE:uint8_t{
    LOCAL,
    NETWORKED
};

class Pyro{

    public:
        Pyro(uint8_t id,uint16_t duration,SystemStatus& systemstatus,PYROTYPE pyrotype);     
        /**
         * @brief Check continiuty of ematch and update the internal continuity state
         * 
         * @return true 
         * @return false 
         */
        virtual void check_continuity() = 0; 
        /**
         * @brief Fire the pyro
         * 
         */
        virtual void doStuff();
        /**
         * @brief Get the Continuity status
         * 
         * @return true 
         * @return false 
         */
        bool getContinuity(){return _continuity;};

        /**
         * @brief Set the how long the ematch is fired for
         * 
         * @param duration 
         */
        void setDuration(uint16_t duration){_fireDuration = duration;};

        PYROTYPE getType(){return _pyrotype;};
        
        virtual ~Pyro(){};

    protected:
        uint8_t _id; // pyro id for logging purposes
        uint16_t _fireDuration; // how long we fire the ematch for
        SystemStatus& _systemstatus; // pointer to system status object
        PYROTYPE _pyrotype;
        bool _continuity; 
        
        
        

       
};

#endif