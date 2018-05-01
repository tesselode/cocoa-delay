#include "StatefulDrive.h"

double StatefulDrive::Process(double input, double amount, double edge)
{
	double driven = atan(input * amount) / amount;
	value += (input - value) * edge;
	value += (driven - value) * edge;
	return value;
}
