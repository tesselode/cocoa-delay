#ifndef __DELAY__
#define __DELAY__

#include <cmath>
#include "Filter.h"
#include "Knob.h"
#include "StatefulDrive.h"
#include "Util.h"
#include <vector>
#include "IPlug_include_in_plug_hdr.h"

enum PanModes
{
	stationary,
	pingPong,
	circular,
	numPanModes
};

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
	void UpdateReadPositions();
	void UpdateWritePosition();
	void UpdateParameters();
	void UpdateLfo();
	void UpdateDrift();
	double GetSample(std::vector<double> &buffer, double position);

	IGraphics* pGraphics;
	double dt = 0.0;

	// delay
	std::vector<double> bufferL;
	std::vector<double> bufferR;
	int writePosition;
	double readPositionL;
	double readPositionR;

	// fading parameters
	PanModes currentPanMode = PanModes::stationary;
	double parameterChangeVolume = 1.0;
	double stationaryPanAmount = 0.0;
	double circularPanAmount = 0.0;

	// filters
	OnePoleFilter lp1L;
	OnePoleFilter lp1R;
	TwoPoleFilter lp2L;
	TwoPoleFilter lp2R;
	FourPoleFilter lp4L;
	FourPoleFilter lp4R;
	StateVariableFilter lpSvfL;
	StateVariableFilter lpSvfR;
	OnePoleFilter hpL;
	OnePoleFilter hpR;

	// drive
	StatefulDrive statefulDrive;
	TwoPoleFilter driveFilterL;
	TwoPoleFilter driveFilterR;

	// modulation
	double lfoPhase = 0.0;
	double driftVelocity = 0.0;
	double driftPhase = 0.0;
};

#endif
