#include "Lofi.h"

double Lofi::Process(double dt, double input, double speed)
{
	phase += dt;
	while (phase >= speed)
	{
		phase -= speed;
		previous = current;
		current = input;
	}
	auto position = phase == 0.0 ? 0.0 : sin(phase * phase) / phase;
	return current + (previous - current) * position;
}
