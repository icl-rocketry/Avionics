#pragma once
//object representing event
#include <memory>
#include <functional>
#include <string>

#include "condition.h"

#include "Storage/logController.h"


using condition_t = std::function<bool()>;
using action_t = std::function<void()>;



class Event{
    public:
        Event(int eventID,condition_t condition,action_t action,bool singleUse,uint16_t actionCooldown,LogController& logcontroller):
        _eventID(eventID),
        _condition(std::move(condition)),
        _action(std::move(action)),
        _singleUse(singleUse),
        _cooldown(actionCooldown),
        _logcontroller(logcontroller),
        _timeTriggered(0), // initialize to zero as no event can be triggered at 'zero' time
        _lastActionTime(0),
        _previouslyFired(false)
        {
            _logcontroller.log("Event " + std::to_string(_eventID) + " created");
        };

        void update();

        uint32_t timeTriggered(){return _timeTriggered;};
    
    private:

        const int _eventID;
        const condition_t _condition;
        const action_t _action;
        const bool _singleUse;
        const uint16_t _cooldown; // dont know how useful this will be

        LogController& _logcontroller;

        uint32_t _timeTriggered;
        uint32_t _lastActionTime;

        bool _previouslyFired;
        



};

