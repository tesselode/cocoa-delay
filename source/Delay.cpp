#include "Delay.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int numPrograms = 128;
const int tapeLength = 10;

enum Parameters
{
	delayTime,
	tempoSync,
	tempoSyncTime,
	feedback,
	wetVolume,
	numParameters
};

enum TempoSyncTimes
{
	whole,
	dottedHalf,
	half,
	tripletHalf,
	dottedQuarter,
	quarter,
	tripletQuarter,
	dottedEighth,
	eighth,
	tripletEighth,
	dottedSixteenth,
	sixteenth,
	tripletSixteenth,
	dottedThirtysecond,
	thirtysecond,
	tripletThirtysecond,
	dottedSixtyforth,
	sixtyforth,
	tripletSixtyforth,
	numTempoSyncTimes
};

void Delay::InitParameters()
{
	GetParam(Parameters::delayTime)->InitDouble("Delay time", 1.0, 0.001, 2.0, .01, "", "", 2.0);
	GetParam(Parameters::tempoSync)->InitBool("Tempo sync", false);
	GetParam(Parameters::tempoSyncTime)->InitEnum("Tempo sync delay time", TempoSyncTimes::quarter, TempoSyncTimes::numTempoSyncTimes);
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

double Delay::GetDelayTime()
{
	
	switch ((bool)GetParam(Parameters::tempoSync)->Value())
	{
	case true:
	{
		auto beatLength = 60 / GetTempo();
		switch ((TempoSyncTimes)(int)GetParam(Parameters::tempoSyncTime)->Value())
		{
		case whole: return beatLength * 4;
		case dottedHalf: return beatLength * 3;
		case half: return beatLength * 2;
		case tripletHalf: return beatLength * 4/3;
		case dottedQuarter: return beatLength * 3/2;
		case quarter: return beatLength * 1;
		case tripletQuarter: return beatLength * 2/3;
		case dottedEighth: return beatLength * 3/4;
		case eighth: return beatLength * 1/2;
		case tripletEighth: return beatLength * 1/3;
		case dottedSixteenth: return beatLength * 3/8;
		case sixteenth: return beatLength * 1/4;
		case tripletSixteenth: return beatLength * 1/6;
		case dottedThirtysecond: return beatLength * 3/16;
		case thirtysecond: return beatLength * 1/8;
		case tripletThirtysecond: return beatLength * 1/12;
		case dottedSixtyforth: return beatLength * 3/32;
		case sixtyforth: return beatLength * 1/16;
		case tripletSixtyforth: return beatLength * 1/24;
		}
	}
	case false:
		return GetParam(Parameters::delayTime)->Value();
	}
}

double Delay::GetTargetReadPosition()
{
	return GetSampleRate() * GetDelayTime();
}

void Delay::InitBuffer()
{
	buffer.clear();
	for (int i = 0; i < GetSampleRate() * tapeLength; i++)
		buffer.push_back(0.0);
	writePosition = 0.0;
	readPosition = GetTargetReadPosition();
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

double Delay::GetBuffer(double position)
{
	while (position < 0.0) position += std::size(buffer);
	while (position > std::size(buffer)) position -= std::size(buffer);
	auto positionA = floor((float)position);
	auto positionB = ceil((float)position);
	auto sampleA = buffer[positionA];
	auto sampleB = buffer[positionB];
	return sampleA + (sampleB - sampleA) * fmod(position, 1.0);
}

void Delay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		readPosition += (GetTargetReadPosition() - readPosition) * 10.0 / GetSampleRate();

		auto in = inputs[0][s] + inputs[1][s];

		auto delayOut = GetBuffer(writePosition - readPosition);
		buffer[writePosition] = in + delayOut * GetParam(Parameters::feedback)->Value();
		writePosition += 1;
		writePosition %= std::size(buffer);

		outputs[0][s] = inputs[0][s] + delayOut * GetParam(Parameters::wetVolume)->Value();
		outputs[1][s] = inputs[1][s] + delayOut * GetParam(Parameters::wetVolume)->Value();
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
}
