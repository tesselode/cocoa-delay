#pragma once

#include <cmath>

enum DriveStyles
{
	spiralDrive,
	sinDrive,
	atanDrive,
	tanhDrive,
	sqrtDrive,
	atanCosDrive,
	numDriveStyles
};

class StatefulDrive
{
public:
	double Process(double input, DriveStyles style);

private:
	double previous = 0.0;
};

