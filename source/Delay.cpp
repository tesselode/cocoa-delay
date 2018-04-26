#include "Delay.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int numPrograms = 128;
const int maxDelayTime = 2;

enum Parameters
{
	delayTime,
	feedback,
	wetVolume,
	numParameters
};

void Delay::InitParameters()
{
	GetParam(Parameters::delayTime)->InitDouble("Delay time", 1.0, 0.1, maxDelayTime, .01);
	GetParam(Parameters::feedback)->InitDouble("Feedback amount", 0.5, 0.0, 1.0, .01);
	GetParam(Parameters::wetVolume)->InitDouble("Wet volume", .5, 0.0, 1.0, .01);
}

void Delay::InitGraphics()
{
	IGraphics* pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachPanelBackground(&COLOR_GRAY);

	IBitmap knob = pGraphics->LoadIBitmap(KNOB_ID, KNOB_FN, 60);

	AttachGraphics(pGraphics);
}

void Delay::InitPresets()
{
	MakeDefaultPreset("-", numPrograms);
}

void Delay::InitBuffer()
{
	buffer.clear();
	for (int i = 0; i < GetSampleRate() * maxDelayTime; i++)
		buffer.push_back(0.0);
	readPosition = 0;
	writePosition = GetSampleRate() * GetParam(Parameters::delayTime)->Value();
	writePosition %= std::size(buffer);
}

Delay::Delay(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, numPrograms, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
}

Delay::~Delay() {}

void Delay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		auto in = inputs[0][s] + inputs[1][s];

		auto out = in + buffer[readPosition] * GetParam(Parameters::feedback)->Value();
		buffer[writePosition] = out;

		readPosition += 1;
		readPosition %= std::size(buffer);
		writePosition += 1;
		writePosition %= std::size(buffer);

		outputs[0][s] = inputs[0][s] + out * GetParam(Parameters::wetVolume)->Value();
		outputs[1][s] = inputs[1][s] + out * GetParam(Parameters::wetVolume)->Value();
	}
}

void Delay::Reset()
{
	TRACE;
	IMutexLock lock(this);
	InitBuffer();
}

void Delay::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
	case delayTime:
		writePosition = readPosition + GetSampleRate() * GetParam(Parameters::delayTime)->Value();
		writePosition %= std::size(buffer);
		break;
	}
}
