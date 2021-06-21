#ifndef EVENT_H
#define EVENT_H
//object representing event

#include "condition.h"

//DUMMY CLASS // UR A DUMMY CLASS
class Pyro {
public:
    void doStuff() {}
};
//DUMMY CLASS


class Event{
public:
    Event(Pyro &pyro, Condition& cond) : 
    _cond(cond),
    _pyro(pyro)  
    {};

    void update();

private:
    Condition& _cond;
    Pyro& _pyro;
    
    void _fire();
};


/*
Pyro pyrotest;
Time_Since_Launch fv;
ConditionFlightVar testcondition(fv,Operator::GT,100);

Event test(pyrotest,testcondition);*/

#endif