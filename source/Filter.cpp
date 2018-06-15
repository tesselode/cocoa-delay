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

void MultiFilter::SetMode(FilterModes m)
{
	if (currentMode != m)
	{
		previousMode = currentMode;
		currentMode = m;
		crossfading = true;
		currentModeMix = 0.0;
	}
}

void MultiFilter::Process(double dt, double & l, double & r, double cutoff, bool highPass)
{
	switch (crossfading)
	{
	case true:
	{
		currentModeMix += 100.0 * dt;
		if (currentModeMix >= 1.0)
		{
			auto current = currentMode;
			currentModeMix = 1.0;
			crossfading = false;
			filters[previousMode]->Reset();
		}

		auto inL = l;
		auto inR = r;
		auto tempOutL = 0.0;
		auto tempOutR = 0.0;
		l = 0.0;
		r = 0.0;

		filters[previousMode]->Process(dt, inL, inR, cutoff, tempOutL, tempOutR, highPass);
		l += tempOutL * (1.0 - currentModeMix);
		r += tempOutR * (1.0 - currentModeMix);
		filters[currentMode]->Process(dt, inL, inR, cutoff, tempOutL, tempOutR, highPass);
		l += tempOutL * currentModeMix;
		r += tempOutR * currentModeMix;

		break;
	}
	case false:
		filters[currentMode]->Process(dt, l, r, cutoff, l, r, highPass);
		break;
	}
}
