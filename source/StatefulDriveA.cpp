#include "StatefulDriveA.h"

double StatefulDriveA::Process(double input, double amount)
{
	double driven = atan(input * amount) / amount;
	value += (input - value) * .9;
	value += (driven - value) * .5;
	return value;
}
