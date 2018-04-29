#pragma once

#include <cmath>

const double pi = 2 * acos(0.0);

// https://stackoverflow.com/a/707426
inline int wrap(int kX, int const kLowerBound, int const kUpperBound)
{
	int range_size = kUpperBound - kLowerBound + 1;

	if (kX < kLowerBound)
		kX += range_size * ((kLowerBound - kX) / range_size + 1);

	return kLowerBound + (kX - kLowerBound) % range_size;
}

// musicdsp.org hermite interpolation
inline float interpolate(float x, float y0, float y1, float y2, float y3)
{
	// 4-point, 3rd-order Hermite (x-form) 
	float c0 = y1;
	float c1 = 0.5f * (y2 - y0);
	float c2 = y0 - 2.5f * y1 + 2.f * y2 - 0.5f * y3;
	float c3 = 1.5f * (y1 - y2) + 0.5f * (y3 - y0);
	return ((c3 * x + c2) * x + c1) * x + c0;
}

inline void adjustStereoWidth(double inL, double inR, double width, double & outL, double & outR)
{
	auto mid = (inL + inR) * .5;
	auto side = (inL - inR) * .5;
	side *= width;
	outL = mid + side;
	outR = mid - side;
}

inline void adjustPanning(double inL, double inR, double angle, double &outL, double &outR)
{
	auto c = cos(angle);
	auto s = sin(angle);
	outL = inL * c - inR * s;
	outR = inL * s + inR * c;
}

enum DriveStyles
{
	absDrive,
	tanhDrive,
	atanDrive,
	sqrtDrive,
	x4Drive,
	cubicDrive,
	sinDrive,
	atanCosDrive,
	numDriveStyles,
};

inline double drive(double x, DriveStyles style, double amount)
{
	x *= amount;
	switch (style)
	{
	case absDrive:
		x = x / (1.0 + abs(x));
		break;
	case tanhDrive:
		x = tanh(x);
		break;
	case atanDrive:
		x = atan(x);
		break;
	case sqrtDrive:
		x = x / sqrt(1.0 + x * x);
		break;
	case x4Drive:
		x = x / (1.0 + x * x * x * x);
		break;
	case cubicDrive:
		if (x <= -1)
			x = -2.0 / 3.0;
		else if (x >= 1)
			x = 2.0 / 3.0;
		else
			x = x - (x * x * x) / 3.0;
		break;
	case sinDrive:
		x = sin(x);
		break;
	case atanCosDrive:
		x = atan(x) * cos(x);
		break;
	}
	x /= amount;
	return x;
}

// random numbers

// https://stackoverflow.com/questions/1640258/need-a-fast-random-generator-for-c
static unsigned long x = 123456789, y = 362436069, z = 521288629;
inline unsigned long xorshift(void)
{
	unsigned long t;
	x ^= x << 16;
	x ^= x >> 5;
	x ^= x << 1;
	t = x;
	x = y;
	y = z;
	z = t ^ x ^ y;
	return z;
}

const double xorshiftMultiplier = 2.0 / ULONG_MAX;
inline double random()
{
	return -1.0 + xorshift() * xorshiftMultiplier;
}