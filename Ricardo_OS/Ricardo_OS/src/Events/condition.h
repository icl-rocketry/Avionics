#pragma once

#include "flightVariable.h"
#include <stdexcept>

enum class Operator:uint8_t {
	AND,
	OR,
	LT, // Less than
	GT // Greater than
};


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
	
	ConditionCondition(Condition& cond1, Operator op, Condition& cond2):
	_cond1(cond1), _cond2(cond2), _op(op) {
		(op == Operator::LT || op == Operator::GT) ? throw std::invalid_argument("Invalid operator type") : 0;
	}

	~ConditionCondition() {};



	bool check();

private:
	Condition& _cond1;
	Condition& _cond2;
	Operator _op;

};

class ConditionFlightVar: public Condition {
public:
	
	ConditionFlightVar(FlightVariable& var, Operator op, double threshold):
	_var(var), _threshold(threshold), _op(op) {
		(op == Operator::AND || op == Operator::OR) ? throw std::invalid_argument("Invalid operator type") : 0;
	};

	~ConditionFlightVar() {};

	bool check();

private:
	FlightVariable& _var;
	double _threshold;
	Operator _op;
};