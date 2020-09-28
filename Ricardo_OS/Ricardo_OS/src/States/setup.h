/*
State run on startup of board. Runs initialzation of most things

*/

#ifndef SETUP_H
#define SETUP_H
#include "Arduino.h"
#include "state.h"

class Setup: public State{
    public:
        Setup(stateMachine *sm);
        void initialise();
        State* update();
        void exitstate();


};


#endif