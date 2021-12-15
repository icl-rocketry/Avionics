#pragma once

#include <optional>
#include <variant>
#include <functional>
#include <unordered_map>

#include "eventhandler.h"

using flightvariable_t = std::optional<std::variant<float,uint32_t>>;
using flightvariablefunc_t = std::function<flightvariable_t(int)>;

class FlightVariables;
// using memberfunc = std::function<flightvariable_t(FlightVariables*,int)>;
using memberfunc_t = flightvariable_t (FlightVariables::*)(int);

class FlightVariables{ 
    public:
    
        /**
         * @brief Get function pointer for given flight variable name
         * 
         * @param funcName 
         * @return flightvariablefunc_t 
         */
        flightvariablefunc_t get(const std::string& funcName);

        /**
         * @brief Get the Instance of flightvariables
         * 
         * @return FlightVariables& 
         */
        static FlightVariables& getInstance();

        FlightVariables(FlightVariables const &) = delete;
        void operator=(FlightVariables const &) = delete;


        /**
         * @brief Set the Launch Time in ms
         * 
         * @param time 
         */
        void setIgnitionTime(uint32_t time){ _ignitionTime = time; };

        /**
         * @brief Set the Launch Time in ms
         * 
         * @param time 
         */
        void setLaunchTime(uint32_t time){ _launchTime = time; };

        /**
         * @brief Set the Apogee Time in ms
         * 
         * @param time 
         */
        void setApogeeTime(uint32_t time){ _apogeeTime = time; };

    private:
        /**
         * @brief pointer to event handler
         * 
         */
        EventHandler* _eventhandler;

        uint32_t _ignitionTime;
        uint32_t _launchTime;
        uint32_t _apogeeTime;
        
        flightvariable_t TimeSinceIgnition(int arg=0);
        flightvariable_t TimeSinceLaunch(int arg=0);
        flightvariable_t TimeSinceApogee(int arg=0);
        /**
         * @brief Get Time since Event
         * 
         * @param arg Event ID
         * @return flightvariable_t 
         */
        flightvariable_t TimeSinceEvent(int arg);
        /**
         * @brief Position
         * 
         * @param arg 0:North,1:East,2:Down,3:Magnitude
         * @return flightvariable_t 
         */
        flightvariable_t Position(int arg);
        /**
         * @brief Velocity
         * 
         * @param arg 0:North,1:East,2:Down,3:Magnitude
         * @return flightvariable_t 
         */
        flightvariable_t Velocity(int arg);
        /**
         * @brief Acceleration
         * 
         * @param arg 0:North,1:East,2:Down,3:Magnitude
         * @return flightvariable_t 
         */
		flightvariable_t Acceleration(int arg);

		FlightVariables() = default;

        static const std::unordered_map<std::string, memberfunc_t> function_map; // constexpr in the future smh

};

