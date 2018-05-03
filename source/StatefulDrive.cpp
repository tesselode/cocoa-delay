#include "StatefulDrive.h"

double StatefulDrive::Process(double dt, double input, double amount, double edge)
{
	edge *= 44100 * dt;
	edge = edge > 1.0 ? 1.0 : edge;
	value += (input - value) * edge;
	double driven = atan(value * amount) / amount;
	value += (driven - value) * edge;
	return value;
}
