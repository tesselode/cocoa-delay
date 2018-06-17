#pragma once

#include <cmath>

class Lofi
{
public:
	double Process(double dt, double input, double speed);

private:
	double phase = 0.0;
	double previous = 0.0;
	double current = 0.0;
};

