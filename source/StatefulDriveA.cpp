#include "StatefulDriveA.h"

double StatefulDriveA::Process(double input, double amount, double edge)
{
	double driven = atan(input * amount) / amount;
	value += (input - value) * edge;
	value += (driven - value) * edge;
	return value;
}
