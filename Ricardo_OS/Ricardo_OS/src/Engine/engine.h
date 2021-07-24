#pragma once

// Abstract interface to engines

enum class ENGINE_STATE:uint8_t{
    NOMINAL,
    ABNORMAL
};

class stateMachine;

class Engine{
    public:
        Engine(stateMachine* sm); // logcontroller systemstatus networkmanager

        void update(); //????  

        bool iginition();
        int getStatus();
        bool shutdown();

    private:
        stateMachine* _sm;
        ENGINE_STATE _engine_state;
};



