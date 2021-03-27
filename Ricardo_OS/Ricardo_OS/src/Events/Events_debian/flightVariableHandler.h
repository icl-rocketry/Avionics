#pragma once

#include <vector>
#include <array>
#include "flightVariable.h"
//object handles intialization and management of flight variable
//default flight variables

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
        FlightVariableHandler();

        FlightVariable& operator[](FLIGHT_VARIABLE flightvariable);

        void update(); // update all flight variables


    private:
        std::array<FlightVariable&,static_cast<unsigned int>(FLIGHT_VARIABLE::__COUNT)> flightvariables; // array containing references to flight variables

};

