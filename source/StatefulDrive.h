#pragma once

#include <cmath>

class StatefulDrive
{
public:
	double Process(double input, double amount, double edge);

private:
	double value = 0.0;
};

