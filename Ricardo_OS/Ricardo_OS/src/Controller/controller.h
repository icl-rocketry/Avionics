#pragma once

#include <chrono>
#include <Arduino.h>

#include "controllable.h"
#include "Storage/logController.h"

#include "Sensors/sensorStructs.h"


// Controller class which computes an output for a controllable
// based on a control law

class Controller
{
public:
	/**
	 * @brief Calls the calculation method when the correct time is satisfied. Can be overriden in
	 * derived if more precise timing is required so a hardware timer is used
	 *
	 */
	virtual void update(const SensorStructs::state_t& estimator_state);

	virtual ~Controller() = 0;

	uint8_t getID(){return _id;};

protected:
	Controller(uint8_t id,Controllable *controllable, LogController &logcontroller, uint32_t update_interval = 0)
		: _id(id), _controllable(controllable), _logcontroller(logcontroller), _update_interval(update_interval),
		  _last_update(millis())
	{
	}

	/**
	 * @brief Calculates the output for a given input, defined by the derived class only
	 *
	 */
	virtual void calculate(const SensorStructs::state_t& estimator_state) = 0;

	const uint8_t _id;

	Controllable * const _controllable;
	LogController &_logcontroller;
	// Minimum time between controller calculations
	// If update is called such that now - last_update < update_interval
	// update is a no-op
	const uint32_t _update_interval;
	
	uint32_t _last_update;
	

	uint32_t deltaT;

	
};

