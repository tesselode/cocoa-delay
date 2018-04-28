#pragma once

class Filter
{
public:
	void Process(double inL, double inR, double cutoff, double &outL, double &outR);

private:
	double left = 0.0;
	double right = 0.0;
};

