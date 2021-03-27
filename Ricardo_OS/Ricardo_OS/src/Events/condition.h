#pragma once

#include "flightVariable.h"

enum Operator {
	AND,
	OR
};


class Condition {

public:
	//Condition(FlightVariable& var, Operator op, double threshold);
	//Condition(FlightVariable& var, Operator op, FlightVariable& var2);

	virtual bool check() = 0;
    virtual ~Condition() = 0;
};


class ConditionCondition: public Condition {
public:
	ConditionCondition(Condition& cond1, Operator op, Condition& cond2);

	bool check();

private:
	Condition& _cond1;
	Condition& _cond2;
	Operator _op;

};