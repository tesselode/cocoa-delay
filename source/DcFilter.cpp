#include "DcFilter.h"

double DcFilter::Process(double input)
{
	auto output = input - previousInput + .995 * previousOutput;
	previousInput = input;
	previousOutput = output;
	return output;
}
