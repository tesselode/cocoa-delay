#pragma once

#include <cmath>

class StatefulDrive
{
public:
	double Process(double input, double amount);

private:
	double previous = 0.0;
};

