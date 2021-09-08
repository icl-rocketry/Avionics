/* 
state for groundstation use as reciever
*/
#ifndef GROUNDSTATION_H
#define GROUNDSTATION_H

#include "state.h"

class Groundstation: public State {
    public:
        Groundstation(stateMachine* sm);
        void initialise();
        State* update();
        void exitstate();

};

#endif