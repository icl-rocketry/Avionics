#pragma once

#include "flightVariable.h"
#include <stdexcept>
#include <memory>
#include <functional>

enum class Operator:uint8_t {
	AND,
	OR,
	LT, // Less than
	GT // Greater than
};

////////////////////////////////////////////////////////
//		1. We can use std::function in place of Operator, makes the code cleaner...
//		2. Why are we even bothering with flight variables? 
//		   We can just use member variable pointers which we get from state estimator
//		3. I feel like we could get rid of ConditionCondition and ConditionFlightVar
//		   with some clever templating

class Condition {

	public:
		//Condition(FlightVariable& var, Operator op, double threshold);
		//Condition(FlightVariable& var, Operator op, FlightVariable& var2);

		virtual bool check() = 0;
		virtual ~Condition() = 0;
};

inline Condition::~Condition(){};

class ConditionCondition: public Condition {
public:
	
	ConditionCondition(std::unique_ptr<Condition> cond1, std::function<bool(bool, bool)> op, std::unique_ptr<Condition> cond2):
	_cond1(std::move(cond1)),//move ownership to the condition object
	_cond2(std::move(cond2)),
	_op(std::move(op))
	{ }



	// Condition& _cond2;
	std::unique_ptr<Condition> _cond1;
	std::unique_ptr<Condition> _cond2;

	std::function<bool(bool, bool)> _op;

};

class ConditionFlightVar: public Condition {
public:
	
	ConditionFlightVar(const FlightVariable& var, std::function<bool(double, double)> op, double threshold):
	_var(var),
	_threshold(threshold),
	_op(std::move(op))
	{ };

	~ConditionFlightVar() {};

	bool check();

private:
	const FlightVariable& _var;
	double _threshold;
	std::function<bool(double, double)> _op;
};