#include "Filter.h"

void Filter::Process(double dt, double inL, double inR, double cutoff, double &outL, double &outR)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	left += (inL - left) * cutoff;
	right += (inR - right) * cutoff;
	outL = left;
	outR = right;
}
