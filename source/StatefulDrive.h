#pragma once

#include <cmath>

class StatefulDrive
{
public:
	double Process(double dt, double input, double amount, double edge);

private:
	double value = 0.0;
};

