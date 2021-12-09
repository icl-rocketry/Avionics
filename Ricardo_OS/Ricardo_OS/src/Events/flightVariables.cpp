#include "flightVariables.h"

#include <optional>
#include <variant>
#include <functional>
#include <unordered_map>

const std::unordered_map<std::string, memberfunc> FlightVariables::function_map {
	{"TimeSinceLaunch", &FlightVariables::TimeSinceLaunch},
	{"TimeSinceApogee", &FlightVariables::TimeSinceApogee},
	{"TimeSinceEvent", &FlightVariables::TimeSinceEvent},
	{"Velocity", &FlightVariables::Velocity},
	{"Acceleration", &FlightVariables::Acceleration}
};


