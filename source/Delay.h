#ifndef __DELAY__
#define __DELAY__

#include <cmath>
#include "Filter.h"
#include "Util.h"
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
	double GetDelayTime();
	void GetReadPositions(double & l, double & r);
	void InitBuffer();
	void UpdateDrift();
	double GetSample(std::vector<double> &buffer, double position);

	// delay
	std::vector<double> bufferL;
	std::vector<double> bufferR;
	int writePosition;
	double readPositionL;
	double readPositionR;

	// filters
	Filter lp;
	Filter hp;

	// modulation
	double lfoPhase = 0.0;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
};

#endif
