#include "event.h"
#include "condition.h"
#include "Storage/utils.h"
#include "stdexcept"
#include <string>





Event::~Event(){};

void Event::update(){
	if (_cond->check()){
		if ((_singleFire && !_previouslyFired) || (!_singleFire)){ // ensure that it only fires once if singlefire is true
			_execute();
			_triggerTime = millis();
			_previouslyFired = true;
		}
	}
}


void EngineEvent::_execute(){
	_logcontroller->log("Event(ENGINE):" + std::to_string(_eventID) + " fired");
	return;
}


void PyroEvent::_execute(){
	_logcontroller->log("Event(PYRO):" + std::to_string(_eventID) + " fired");
	_pyrohandler->fire(_id);
}