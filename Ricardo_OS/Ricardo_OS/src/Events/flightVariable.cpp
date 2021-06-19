#include "flightVariable.h"
#include "eventHandler.h"

FlightVariable::FlightVariable(){};
FlightVariable::~FlightVariable(){}; 

// void FlightVariable::update(){

//     // _value = millis() - _launchTime;

// };//check if launch flag triggered and record the time

Time_Since_Launch::Time_Since_Launch(){};
void Time_Since_Launch::update(){
    //check system status for launch flag and record current time to _value
};

double Time_Since_Launch::get(){return millis() - _value;};

Time_Since_Apogee::Time_Since_Apogee(){};        
void Time_Since_Apogee::update(){

    // _value = millis() - _apogeeTime;
    //check system status for apogee flag

};//check if launch flag triggered and record the time

double Time_Since_Apogee::get(){return millis() - _value;};

Time_Since_Event::Time_Since_Event(EventHandler& eventHandler):
_eventHandler(eventHandler)
{};
double Time_Since_Event::get(int arg){
    // milis() - event.timeTriggered;
    return millis() - _eventHandler.timeTriggered(arg);

};

void Time_Since_Event::update(){
//empty method
    // _value = millis() - _apogeeTime;

}; //check if launch flag triggered and record the time

Apogee::Apogee(){};    
void Apogee::update(){

    // check apogee flag from systemstatus and return

};//check if launch flag triggered and record the time
    
Speed::Speed(){};    
void Speed::update(){

    // check apogee flag from systemstatus and return

};//check if launch flag triggered and record the time

Acceleration::Acceleration(){};  
void Acceleration::update(){

    // check apogee flag from systemstatus and return

};//check if launch flag triggered and record the time

Altitude::Altitude(){};  
void Altitude::update(){

    // check apogee flag from systemstatus and return

};//check if launch flag triggered and record the time
    
