#pragma once

class DcFilter
{
public:
	double Process(double input);

private:
	double previousInput = 0.0;
	double previousOutput = 0.0;
};

