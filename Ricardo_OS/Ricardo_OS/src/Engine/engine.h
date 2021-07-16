#ifndef ENGINECONTROLLER_H
#define ENGINECONTROLLER_H

#include <Arduino.h>

enum class ENGINE_STATE:uint8_t{
    NOMINAL,
    ABNORMAL
};

class stateMachine;

class Engine{
    public:
        Engine(stateMachine* sm);


        bool iginition();
        int getStatus();
        bool shutdown();

    private:
        stateMachine* _sm;
        ENGINE_STATE _engine_state;
};




#endif