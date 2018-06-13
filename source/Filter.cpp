#include "Filter.h"

double OnePoleFilter::Process(double dt, double input, double cutoff)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	a += (input - a) * cutoff;
	return a;
}

double TwoPoleFilter::Process(double dt, double input, double cutoff)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	a += (input - a) * cutoff;
	b += (a - b) * cutoff;
	return b;
}

double FourPoleFilter::Process(double dt, double input, double cutoff)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	a += (input - a) * cutoff;
	b += (a - b) * cutoff;
	c += (b - c) * cutoff;
	d += (c - d) * cutoff;
	return d;
}

double StateVariableFilter::Process(double dt, double input, double cutoff)
{
	input *= .9;

	// f calculation
	cutoff *= 8000.0;
	auto f = 2 * sin(pi * cutoff * dt);
	f = f > 1.0 ? 1.0 : f < 0.0 ? 0.0 : f;

	// processing
	auto high = input - (low + band);
	band += f * high;
	low += f * band;

	return low;
}