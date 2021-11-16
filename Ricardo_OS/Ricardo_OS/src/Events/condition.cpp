#include "condition.h"
#include <stdexcept>

bool ConditionCondition::check() {
	if (_cond1 && _cond2) {
		return _op(_cond1->check(), _cond2->check());
	} else {
		throw "something"; // TODO:
	}
}


bool ConditionFlightVar::check() {
	return _op(_var.get(), _threshold);
}