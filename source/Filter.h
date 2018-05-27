#pragma once

#include <cmath>

class OnePoleFilter
{
public:
	double Process(double dt, double input, double cutoff);

private:
	double a = 0.0;
};

class TwoPoleFilter
{
public:
	double Process(double dt, double input, double cutoff);

private:
	double a = 0.0;
	double b = 0.0;
};

class FourPoleFilter
{
public:
	double Process(double dt, double input, double cutoff);

private:
	double a = 0.0;
	double b = 0.0;
	double c = 0.0;
	double d = 0.0;
};

class StateVariableFilter
{
public:
	double Process(double dt, double input, double cutoff);

private:
	double pi = 2 * acos(0.0);
	double band = 0.0;
	double low = 0.0;
};