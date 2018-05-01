#pragma once

#include <cmath>

class StatefulDriveA
{
public:
	double Process(double input, double amount, double edge);

private:
	double value = 0.0;
};

