#pragma once

class Filter
{
public:
	double Process(double dt, double input, double cutoff);

private:
	double value = 0.0;
};

