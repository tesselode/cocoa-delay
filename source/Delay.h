#ifndef __DELAY__
#define __DELAY__

#include <cmath>
#include "Filter.h"
#include <vector>
#include "IPlug_include_in_plug_hdr.h"

class Delay : public IPlug
{
public:
	Delay(IPlugInstanceInfo instanceInfo);
	~Delay();

	void Reset();
	void OnParamChange(int paramIdx);
	void ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames);

private:
	void InitParameters();
	void InitGraphics();
	void InitPresets();
	void InitBuffer();
	double GetDelayTime();
	double GetTargetReadPosition();
	double GetBuffer(std::vector<double> &buffer, double position);
	void ChangeStereoWidth(double inL, double inR, double width, double &outL, double &outR);
	void Pan(double inL, double inR, double p, double & outL, double & outR);

	// delay
	std::vector<double> bufferL;
	std::vector<double> bufferR;
	int writePosition;
	double readPosition;

	// filters
	Filter lp;
	Filter hp;
};

#endif
