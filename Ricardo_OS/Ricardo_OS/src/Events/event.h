#ifndef EVENT_H
#define EVENT_H
//object representing event

#include "condition.h"
#include <memory>
#include "Pyros/pyroHandler.h"
#include "Engine/enginehandler.h"
#include "Storage/logcontroller.h"


//DUMMY CLASS // UR A DUMMY CLASS
// class Pyro {
// public:
//     void doStuff() {}
// };
//DUMMY CLASS



class Event{
    public:
        /**
         * @brief Construct a new Event object
         * 
         * @param id Interface ID
         * @param cond Condition Object
         * @param singleFire Is Multiple firing allowed
         * @param eventID event id
         * @param logcontroller pointer to log controller
         */
        Event(uint8_t id, std::unique_ptr<Condition> cond,bool singleFire,uint8_t eventID,LogController* logcontroller):
        _id(id),
        _cond(std::move(cond)), // transfer ownership
        _singleFire(singleFire),
        _previouslyFired(false),
        _eventID(eventID),
        _logcontroller(logcontroller),
        _triggerTime(0)
        {};

        virtual void update();
        virtual ~Event();
        virtual uint32_t timeTriggered(){return _triggerTime;};

    protected:
        /**
         * @brief id of interface to fire
         * 
         */
        uint8_t _id; 
        std::unique_ptr<Condition> _cond;
        bool _singleFire;
        bool _previouslyFired;
        uint8_t _eventID;
        LogController* _logcontroller;

        uint32_t _triggerTime;

        /**
         * @brief virtual action function for event
         * 
         */
        virtual void _execute() = 0;
        
};

class EngineEvent: public Event{
    public:
    /**
     * @brief Construct a new Engine Event object
     * 
     * @param id Id of engine
     * @param cond Condition object
     * @param singleFire Can this event fire multiple times
     * @param eventID Event id
     * @param enginehandler pointer to engine handler
     * @param logcontroller pointer to log controller
     */
    EngineEvent(uint8_t id,std::unique_ptr<Condition> cond,bool singleFire,uint8_t eventID,EngineHandler* enginehandler,LogController* logcontroller):
    Event(id,std::move(cond),singleFire,eventID,logcontroller),
    _enginehandler(enginehandler)
    {};
    /**
     * @brief Check to see if condition has been met
     * 
     */

    void update(){Event::update();};
    /**
     * @brief Get the Time when the event has been triggered.
     * 
     * @return uint32_t Trigger time in milliseconds. Returns 0 if has not been triggered
     */
    uint32_t timeTriggered(){return Event::timeTriggered();};

    protected:
        EngineHandler* _enginehandler; //pointer to engine handler

        void _execute();

};

/**
 * @brief This represents an event which triggers a pyro directly such as main chutes.
 * 
 */
class PyroEvent: public Event{
    public:
        /**
         * @brief Construct a new Pyro Event object
         * 
         * @param id Pyro interface ID
         * @param cond Condition Object
         * @param singleFire Can this event fire multiple times
         * @param eventID Event ID
         * @param pyrohandler pointer to pyrohandler
         * @param logcontroller pointer to log controller
         */
        PyroEvent(uint8_t id, std::unique_ptr<Condition> cond,bool singleFire, uint8_t eventID, PyroHandler* pyrohandler,LogController* logcontroller):
        Event(id,std::move(cond),singleFire,eventID,logcontroller),
        _pyrohandler(pyrohandler)
        {};

        void update(){Event::update();};

        uint32_t timeTriggered(){return Event::timeTriggered();};

    protected:
        PyroHandler* _pyrohandler;
        
        void _execute();
};





/*
Pyro pyrotest;
Time_Since_Launch fv;
ConditionFlightVar testcondition(fv,Operator::GT,100);

Event test(pyrotest,testcondition);*/

#endif