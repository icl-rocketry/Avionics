#pragma once

#include <vector>
#include <array>
#include <tuple>
#include "flightVariable.h"
#include "eventHandler.h"
#include <functional>
//object handles intialization and management of flight variable
//default flight variables

/*
enum class Kiran {
    Dummy // there is no other case cuz Kiran is a dummy
};
class Pavol{
    bool isPavolDumb(){return true;};
}*/
//dont be dumb, leave this enum to automatically assign values
enum class FLIGHT_VARIABLE:unsigned int{
    Time_Since_Launch,
    Time_Since_Apogee,
    Time_Since_Event,
    Altitude,
    Speed,
    Acceleration,
    Apogee,
    __COUNT // THIS VARIABLE HAS TO BE THE LAST!!

};



class FlightVariableHandler{
    public:
        FlightVariableHandler(EventHandler& eventhandler):
        _eventhandler(eventhandler),
        _time_since_launch(),
        _time_since_apogee(),
        _time_since_event(eventhandler),
        _altitude(),
        _speed(),
        _acceleration(),
        _apogee()
        {};

        //acess operator to retrieve flight variables
        FlightVariable& operator[](FLIGHT_VARIABLE flightvariable);

        void update();


    private:
        EventHandler& _eventhandler;

        //Flight Variable Objects
        Time_Since_Launch _time_since_launch;
        Time_Since_Apogee _time_since_apogee;
        Time_Since_Event _time_since_event;
        Altitude _altitude;
        Speed _speed;
        Acceleration _acceleration;
        Apogee _apogee;

        //container of pointers to flight variables
        std::array<FlightVariable*,static_cast<unsigned int>(FLIGHT_VARIABLE::__COUNT)> _flightVariables{{
            (FlightVariable*)&_time_since_launch,
            (FlightVariable*)&_time_since_apogee,
            (FlightVariable*)&_time_since_event,
            (FlightVariable*)&_altitude,
            (FlightVariable*)&_speed,
            (FlightVariable*)&_acceleration,
            (FlightVariable*)&_apogee
        }}; // array containing references to flight variables

};

