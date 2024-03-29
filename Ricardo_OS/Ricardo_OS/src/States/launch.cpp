
#include "launch.h"
#include "flight.h"

#include "flags.h"
#include "stateMachine.h"

#include "Sound/Melodies/melodyLibrary.h"

Launch::Launch(stateMachine* sm):
State(sm,SYSTEM_FLAG::STATE_LAUNCH)
{};

void Launch::initialise(){
    State::initialise();
    //start telemetry logging here
    _sm->logcontroller.startLogging(LOG_TYPE::TELEMETRY);
    _sm->tunezhandler.play(MelodyLibrary::confirmation);


};

State* Launch::update(){
    

    if (abs(_sm->estimator.getData().acceleration(2)) > 1.5){ // launch acceleration threshold comparison of down acceleration with a threshold of 1.5 g idk if this is okay lol?
        State* flight_ptr = new Flight(_sm);
        return flight_ptr;
    }else{
        return this; //loopy loop
    }
};

void Launch::exitstate(){
    State::exitstate();


};

