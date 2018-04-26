#ifndef __DELAY__
#define __DELAY__

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
	double mGain;
};

#endif
