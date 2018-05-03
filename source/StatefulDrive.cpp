#include "StatefulDrive.h"

double StatefulDrive::Process(double input, double amount, double edge)
{
	value += (input - value) * edge;
	double driven = atan(value * amount) / amount;
	value += (driven - value) * edge;
	return value;
}
