#pragma once

#include <cmath>

class StatefulDrive
{
public:
	double Process(double input);

private:
	double previous = 0.0;
};

