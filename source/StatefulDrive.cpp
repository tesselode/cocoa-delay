#include "StatefulDrive.h"

/*

this is a combo of two airwindows plugins. it uses the behavior of PurestDrive:
https://github.com/airwindows/airwindows/blob/master/plugins/WinVST/PurestDrive/PurestDriveProc.cpp

but with the saturation algorithm of Spiral:
https://github.com/airwindows/airwindows/blob/master/plugins/WinVST/Spiral/SpiralProc.cpp

*/

double StatefulDrive::Process(double input, double amount)
{
	auto driven = input == 0.0 ? 0.0 : sin(input * input) / input;
	auto mix = fabs(previous + driven) * .5 * amount;
	previous = driven;
	return input * (1.0 - mix) + driven * mix;
}
