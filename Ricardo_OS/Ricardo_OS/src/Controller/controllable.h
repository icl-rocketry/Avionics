#pragma once

#include <vector>


class Controllable {
	public:
		virtual void control(std::vector<float> u) = 0;
};