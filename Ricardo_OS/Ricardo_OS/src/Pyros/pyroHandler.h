#ifndef PYROHANDLER_H
#define PYROHANDLER_H

#include "pyro.h"


class stateMachine; //forward declaration

class PyroHandler{
    public:
        PyroHandler(stateMachine* sm);

        void setup();
        void update(); // update pyro status 
        bool fire(uint8_t id);
    
    private:
        stateMachine* _sm; //pointer to statemachine

};

#endif