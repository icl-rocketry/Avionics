

#include <vector>
#include <array>
#include "flightVariable.h"
#include "eventHandler.h"
#include "flightVariableHandler.h"

//object handles intialization and management of flight variable
//default flight variables



FlightVariable& FlightVariableHandler::operator[](FLIGHT_VARIABLE flightvariable){
    return *_flightVariables[static_cast<int>(flightvariable)];
};

void FlightVariableHandler::update() { // update all flight variables
    for (FlightVariable* var : _flightVariables) { 
        var->update(); //call the update
    }
};

