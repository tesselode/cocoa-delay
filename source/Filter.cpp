#include "Filter.h"

void Filter::Process(double inL, double inR, double cutoff, double &outL, double &outR)
{
	left += (inL - left) * cutoff;
	right += (inR - right) * cutoff;
	outL = left;
	outR = right;
}
