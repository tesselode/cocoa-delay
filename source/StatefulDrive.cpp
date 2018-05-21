#include "StatefulDrive.h"

/*

this is a combo of two airwindows plugins. it uses the behavior of PurestDrive:
https://github.com/airwindows/airwindows/blob/master/plugins/WinVST/PurestDrive/PurestDriveProc.cpp

but with the saturation algorithm of Spiral:
https://github.com/airwindows/airwindows/blob/master/plugins/WinVST/Spiral/SpiralProc.cpp

*/

double StatefulDrive::Process(double input, DriveStyles style)
{
	double driven;
	switch (style)
	{
	case spiralDrive:
		driven = input == 0.0 ? 0.0 : sin(input * abs(input)) / abs(input);
		break;
	case sinDrive:
		driven = sin(input);
		break;
	case atanDrive:
		driven = atan(input);
		break;
	case tanhDrive:
		driven = tanh(input);
		break;
	case sqrtDrive:
		driven = input / sqrt(1.0 + input * input);
		break;
	case atanCosDrive:
		driven = atan(input) * cos(input);
		break;
	default:
		driven = 0.0;
		break;
	}
	auto mix = fabs(previous + driven) * .5;
	previous = driven;
	return input * (1.0 - mix) + driven * mix;
}
