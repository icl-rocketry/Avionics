#include "event.h"

#include <string>
#ifdef ARDUINO
#include <Arduino.h>
#else
#include "stubs.h"
#endif

void Event::update()
{
	if (_singleUse && _previouslyFired)
	{
		return; //no need to continue evaluating the contiions
	}

	if (_condition())
	{ //check if condition has been met

		//update time triggered
		_timeTriggered = millis();

		//update _previouslyFired flag
		_previouslyFired = true;

		// execute action
		if (millis() - _lastActionTime > _cooldown)
		{
			_action();
			_lastActionTime = millis();
			_logcontroller.log("Event:" + std::to_string(_eventID) + " fired at " + std::to_string(_timeTriggered));
		}
	}
}
