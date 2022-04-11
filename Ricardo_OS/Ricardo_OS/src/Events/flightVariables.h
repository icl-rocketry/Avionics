#pragma once

#include <optional>
#include <variant>
#include <functional>
#include <unordered_map>

#include <Eigen/Core>
#include <Eigen/Eigen>
#include <Eigen/Geometry>

#include "Sensors/sensorStructs.h"


//forward declarations
class FlightVariables;
class EventHandler;

using flightVariable_t = std::optional<float>;
using flightVariableFunc_t = std::function<flightVariable_t(int)>;
using memberFunc_t = flightVariable_t (FlightVariables::*)(int);


class FlightVariables{ 
    public:

        FlightVariables(const SensorStructs::state_t& state, EventHandler& event_handler) : 
        _state(state),
        _eventhandler(event_handler)
        {}

        /**
         * @brief Get function pointer for given flight variable name
         * 
         * @param funcName 
         * @return flightVariableFunc_t 
         */
        flightVariableFunc_t get(const std::string& funcName);

    private:
        const SensorStructs::state_t& _state;
        /**
         * @brief pointer to event handler
         * 
         */
        EventHandler& _eventhandler;

        flightVariable_t TimeSinceIgnition(int arg=0);
        flightVariable_t TimeSinceLaunch(int arg=0);
        flightVariable_t TimeSinceApogee(int arg=0);
        /**
         * @brief Get Time since Event
         * 
         * @param arg Event ID
         * @return flightVariable_t 
         */
        flightVariable_t TimeSinceEvent(int arg);
        /**
         * @brief Position
         * 
         * @param arg 0:North,1:East,2:Down,3:Magnitude
         * @return flightVariable_t 
         */
        flightVariable_t Position(int arg);
        /**
         * @brief Velocity
         * 
         * @param arg 0:North,1:East,2:Down,3:Magnitude
         * @return flightVariable_t 
         */
        flightVariable_t Velocity(int arg);
        /**
         * @brief Acceleration
         * 
         * @param arg 0:North,1:East,2:Down,3:Magnitude
         * @return flightVariable_t 
         */
		flightVariable_t Acceleration(int arg);

        /**
         * @brief Helpfer function to get component of a eigen vector
         * 
         * @param var Eigen Vector
         * @param arg 
         * @return float 
         */
        std::optional<float> getComponent(const Eigen::Vector3f& var, int arg);

        std::optional<float> timeSince(const uint32_t time);

        static const std::unordered_map<std::string, memberFunc_t> function_map; // constexpr in the future smh

};

