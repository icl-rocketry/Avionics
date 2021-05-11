#ifndef ESTIMATOR_H
#define ESTIMATOR_H
#include <Arduino.h>
#include "sensorStructs.h"
#include "MadgwickAHRS.h"
//#include "sensors.h"





class stateMachine; //forward declaration

class Estimator{
    public:
        Estimator(stateMachine* sm);   
        
        void setup();
        void update();

        state_t state;
    private:
        stateMachine* _sm;//pointer to statemachine object

        Madgwick madgwick; // madgwick filter object


};


#endif