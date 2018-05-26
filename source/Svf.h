#pragma once

#include <cmath>

class Svf
{
public:
	double Process(double dt, double input, double cutoff);

private:
	double pi = 2 * acos(0.0);
	double band = 0.0;
	double low = 0.0;
};

