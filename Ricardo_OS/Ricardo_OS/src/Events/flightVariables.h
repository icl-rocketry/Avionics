#pragma once

#include <optional>
#include <variant>
#include <functional>
#include <unordered_map>

using flightvariable_t = std::optional<std::variant<float,uint32_t>>;
using flightvariablefunc_t = std::function<flightvariable_t(int)>;

class FlightVariables;
using memberfunc = std::function<flightvariable_t(FlightVariables*,int)>;
class FlightVariables{
    public:
    
	flightvariablefunc_t get(const std::string& funcName);

    private:
        flightvariable_t TimeSinceLaunch(int arg=0);
        flightvariable_t TimeSinceApogee(int arg=0);
        flightvariable_t TimeSinceEvent(int arg=0);
        flightvariable_t Velocity(int arg=0);
		flightvariable_t Acceleration(int arg=0);

		FlightVariables() = default;
	
	
    static const std::unordered_map<std::string, memberfunc> function_map;

};

