#include "flightVariables.h"

#include <Arduino.h>
#include <optional>
#include <variant>
#include <functional>
#include <unordered_map>

const std::unordered_map<std::string, memberfunc_t> FlightVariables::function_map {
	{"TimeSinceLaunch", &FlightVariables::TimeSinceLaunch},
	{"TimeSinceApogee", &FlightVariables::TimeSinceApogee},
	{"TimeSinceEvent", &FlightVariables::TimeSinceEvent},
	{"Velocity", &FlightVariables::Velocity},
	{"Acceleration", &FlightVariables::Acceleration}
};

// Singleton "constructor"
FlightVariables& FlightVariables::getInstance() {
	static FlightVariables instance;

	return instance;
}

flightvariable_t FlightVariables::TimeSinceIgnition(int arg) 
{
	if (!_ignitionTime){
		return {};
	}
	uint32_t currTime = millis();
	if (currTime < _ignitionTime){ //something has gone wrong
		return{}; 
	}
	return {{static_cast<uint32_t>(currTime-_ignitionTime)}};
}

flightvariable_t FlightVariables::TimeSinceLaunch(int arg) 
{
	if (!_launchTime){
		return {};
	}
	uint32_t currTime = millis();
	if (currTime < _launchTime){ //something has gone wrong
		return{}; 
	}
	return {{static_cast<uint32_t>(currTime-_launchTime)}};
}

flightvariable_t FlightVariables::TimeSinceApogee(int arg) 
{
	if (!_apogeeTime){
		return {};
	}
	uint32_t currTime = millis();
	if (currTime < _apogeeTime){ //something has gone wrong
		return{}; 
	}
	return {{static_cast<uint32_t>(currTime-_apogeeTime)}};
}

flightvariable_t FlightVariables::TimeSinceEvent(int arg) 
{

	uint32_t eventTime = _eventhandler->timeTriggered(arg);

	if (!eventTime){
		return {};
	}
	uint32_t currTime = millis();
	if (currTime < eventTime){ //something has gone wrong
		return{}; 
	}
	return {{static_cast<uint32_t>(currTime-eventTime)}};

}

flightvariable_t FlightVariables::Position(int arg) 
{
	return {};
}

flightvariable_t FlightVariables::Velocity(int arg) 
{
	return {};
}

flightvariable_t FlightVariables::Acceleration(int arg) 
{
	return {};
}


flightvariablefunc_t FlightVariables::get(const std::string& funcName) {
	// returns a lambda that binds the member function pointer obtained from function_map to a singleton instance
	return [&funcName](int idx) {
		// syntax from hell lmao
		return (getInstance().*(function_map.at(funcName)))(idx);
	};
}
