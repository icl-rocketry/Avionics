#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
//global struct for state of rocket




class stateMachine; //forward declaration

class Estimator{
    public:
        Estimator(stateMachine* sm);   
        void setup();
        void update();

        state_t state;
    private:
        stateMachine* _sm;//pointer to statemachine object
        

};


#endif