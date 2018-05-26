#include "Filter.h"

double Filter::Process(double dt, double input, double cutoff)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	value += (input - value) * cutoff;
	return value;
}