#include "condition.h"
#include <stdexcept>

bool ConditionCondition::check() {
	switch (_op) {
	case Operator::AND:
		return _cond1->check() && _cond2->check();
	case Operator::OR:
		return _cond1->check() || _cond2->check();
	default:
		throw std::invalid_argument("Invalid operator type");
		return false;
	}
}


bool ConditionFlightVar::check() {
	switch (_op) {
	case Operator::LT:
		return _var.get() < _threshold;
	case Operator::GT:
		return _var.get() > _threshold;
	default:
		throw std::invalid_argument("Invalid operator type");
		return false;
	}
}