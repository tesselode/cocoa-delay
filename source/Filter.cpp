#include "Filter.h"

double OnePoleFilter::Process(double dt, double input, double cutoff, bool highPass)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	a += (input - a) * cutoff;
	return highPass ? input - a : a;
}

double TwoPoleFilter::Process(double dt, double input, double cutoff, bool highPass)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	a += (input - a) * cutoff;
	b += (a - b) * cutoff;
	return highPass ? input - b : b;
}

double FourPoleFilter::Process(double dt, double input, double cutoff, bool highPass)
{
	cutoff *= 44100 * dt;
	cutoff = cutoff > 1.0 ? 1.0 : cutoff;
	a += (input - a) * cutoff;
	b += (a - b) * cutoff;
	c += (b - c) * cutoff;
	d += (c - d) * cutoff;
	return highPass ? input - d : d;
}

double StateVariableFilter::Process(double dt, double input, double cutoff, bool highPass)
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

	return highPass ? high : low;
}

void MultiFilter::Process(double dt, double & l, double & r, double cutoff, bool highPass)
{
	for (int i = 0; i < std::size(mix); i++)
		mix[i] += ((mode == i ? 1.0 : 0.0) - mix[i]) * 100.0 * dt;

	auto inL = l;
	auto inR = r;
	auto tempOutL = 0.0;
	auto tempOutR = 0.0;
	l = 0.0;
	r = 0.0;

	for (int i = 0; i < std::size(filters); i++)
	{
		filters[i]->Process(dt, inL, inR, cutoff, tempOutL, tempOutR, highPass);
		l += tempOutL * mix[i];
		r += tempOutR * mix[i];
	}
}
