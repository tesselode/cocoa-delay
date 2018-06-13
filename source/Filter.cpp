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

void MultiFilter::UpdateFilterMixes(double dt)
{
	filter1Mix += ((mode == FilterModes::onePole ? 1.0 : 0.0) - filter1Mix) * 100.0 * dt;
	filter2Mix += ((mode == FilterModes::twoPole ? 1.0 : 0.0) - filter2Mix) * 100.0 * dt;
	filter4Mix += ((mode == FilterModes::fourPole ? 1.0 : 0.0) - filter4Mix) * 100.0 * dt;
	filterSvfMix += ((mode == FilterModes::stateVariable ? 1.0 : 0.0) - filterSvfMix) * 100.0 * dt;
}

void MultiFilter::LowPass(double dt, double & l, double & r, double cutoff)
{
	UpdateFilterMixes(dt);

	auto inL = l;
	auto inR = r;
	auto tempOutL = 0.0;
	auto tempOutR = 0.0;

	l = 0.0;
	r = 0.0;
	filter1.Process(dt, inL, inR, cutoff, tempOutL, tempOutR);
	l += tempOutL * filter1Mix;
	r += tempOutR * filter1Mix;
	filter2.Process(dt, inL, inR, cutoff, tempOutL, tempOutR);
	l += tempOutL * filter2Mix;
	r += tempOutR * filter2Mix;
	filter4.Process(dt, inL, inR, cutoff, tempOutL, tempOutR);
	l += tempOutL * filter4Mix;
	r += tempOutR * filter4Mix;
	filterSvf.Process(dt, inL, inR, cutoff, tempOutL, tempOutR);
	l += tempOutL * filterSvfMix;
	r += tempOutR * filterSvfMix;
}