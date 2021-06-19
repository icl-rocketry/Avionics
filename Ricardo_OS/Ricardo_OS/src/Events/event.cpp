#include "event.h"
#include "condition.h"

void Event::update() {
	if (_cond.check()) {
		_fire(); // Fire the pyro
	}
}
void Event::_fire() {
	_pyro.doStuff(); // REPLACE WITH AN ACTUAL METHOD // THIS IS THE ACTUAL METHOD // OK
}