#ifndef PYROHANDLER_H
#define PYROHANDLER_H

#include "pyro.h"
#include <vector>
#include <memory>
#include <ArduinoJson.h>


class stateMachine; //forward declaration

class PyroHandler{
    public:
        PyroHandler(stateMachine* sm);

        void setup(JsonArray pyroConfig);
        void update(); // update pyro status 
        bool fire(uint8_t id);
    
    private:
        stateMachine* _sm; //pointer to statemachine

        std::vector<std::unique_ptr<Pyro> > pyroList; // vector containing pointers to all pyro objects
        
};

#endif