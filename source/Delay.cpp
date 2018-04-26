#include "Delay.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int numPrograms = 128;

enum Parameters
{
	numParameters
};

Delay::Delay(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, numPrograms, instanceInfo)
{
	TRACE;

	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 60);

	AttachGraphics(pGraphics);

	MakeDefaultPreset("-", numPrograms);
}

Delay::~Delay() {}

void Delay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	double* in1 = inputs[0];
	double* in2 = inputs[1];
	double* out1 = outputs[0];
	double* out2 = outputs[1];

	for (int s = 0; s < nFrames; ++s, ++in1, ++in2, ++out1, ++out2)
	{
		*out1 = 0.0;
		*out2 = 0.0;
	}
}

void Delay::Reset()
{
	TRACE;
	IMutexLock lock(this);
}

void Delay::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);
}
