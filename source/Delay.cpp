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
	stereoOffset,
	stereoWidth,
	pan,
	lowPass,
	highPass,
	driveAmount,
	dryVolume,
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
	GetParam(Parameters::delayTime)->InitDouble("Delay time", .2, 0.001, 2.0, .01, "", "", 2.0);
	GetParam(Parameters::tempoSync)->InitBool("Tempo sync", false);
	GetParam(Parameters::tempoSyncTime)->InitEnum("Tempo sync delay time", TempoSyncTimes::quarter, TempoSyncTimes::numTempoSyncTimes);
	GetParam(Parameters::feedback)->InitDouble("Feedback amount", 0.5, 0.0, 1.0, .01);
	GetParam(Parameters::stereoOffset)->InitDouble("Stereo offset", 0.0, -.1, .1, .01);
	GetParam(Parameters::stereoWidth)->InitDouble("Stereo width", 1.0, 0.0, 1.0, .01);
	GetParam(Parameters::pan)->InitDouble("Panning", 0.0, -pi * .5, pi * .5, .01);
	GetParam(Parameters::lowPass)->InitDouble("Low pass", 1.0, 0.0, 1.0, .01);
	GetParam(Parameters::highPass)->InitDouble("High pass", 0.0, 0.0, 1.0, .01);
	GetParam(Parameters::driveAmount)->InitDouble("Drive amount", 0.0, 0.0, 10.0, .01);
	GetParam(Parameters::dryVolume)->InitDouble("Dry volume", 1.0, 0.0, 2.0, .01);
	GetParam(Parameters::wetVolume)->InitDouble("Wet volume", .5, 0.0, 2.0, .01);
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

void Delay::GetReadPositions(double &l, double &r)
{
	auto offset = GetParam(Parameters::stereoOffset)->Value() * .5;
	auto baseTime = GetDelayTime();
	auto timeL = baseTime - offset;
	auto timeR = baseTime + offset;
	l = timeL * GetSampleRate();
	r = timeR * GetSampleRate();
}

void Delay::InitBuffer()
{
	bufferL.clear();
	bufferR.clear();
	for (int i = 0; i < GetSampleRate() * tapeLength; i++)
	{
		bufferL.push_back(0.0);
		bufferR.push_back(0.0);
	}
	writePosition = 0.0;
	GetReadPositions(readPositionL, readPositionR);
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

Delay::Delay(IPlugInstanceInfo instanceInfo)
  :	IPLUG_CTOR(numParameters, numPrograms, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
}

Delay::~Delay() {}

double Delay::GetBuffer(std::vector<double> &buffer, double position)
{
	while (position < 0.0) position += std::size(buffer);
	while (position > std::size(buffer)) position -= std::size(buffer);
	auto positionA = floor((float)position);
	auto positionB = ceil((float)position);
	auto sampleA = buffer[positionA];
	auto sampleB = buffer[positionB];
	return sampleA + (sampleB - sampleA) * fmod(position, 1.0);
}

void Delay::ChangeStereoWidth(double inL, double inR, double width, double & outL, double & outR)
{
	auto mid = (inL + inR) * .5;
	auto side = (inL - inR) * .5;
	side *= width;
	outL = mid + side;
	outR = mid - side;
}

void Delay::Pan(double inL, double inR, double angle, double &outL, double &outR)
{
	auto c = cos(angle);
	auto s = sin(angle);
	outL = inL * c - inR * s;
	outR = inL * s + inR * c;
}

void Delay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		// update read positions
		double targetReadPositionL, targetReadPositionR;
		GetReadPositions(targetReadPositionL, targetReadPositionR);
		readPositionL += (targetReadPositionL - readPositionL) * 10.0 / GetSampleRate();
		readPositionR += (targetReadPositionR - readPositionR) * 10.0 / GetSampleRate();

		// read from buffer
		auto outL = GetBuffer(bufferL, writePosition - readPositionL);
		auto outR = GetBuffer(bufferR, writePosition - readPositionR);
		
		// feedback volume
		outL *= GetParam(Parameters::feedback)->Value();
		outR *= GetParam(Parameters::feedback)->Value();

		// stereo width
		ChangeStereoWidth(outL, outR, GetParam(Parameters::stereoWidth)->Value(), outL, outR);

		// panning
		Pan(outL, outR, GetParam(Parameters::pan)->Value(), outL, outR);

		// filters
		lp.Process(outL, outR, GetParam(Parameters::lowPass)->Value(), outL, outR);
		auto hpOutL = 0.0;
		auto hpOutR = 0.0;
		hp.Process(outL, outR, GetParam(Parameters::highPass)->Value(), hpOutL, hpOutR);
		outL -= hpOutL;
		outR -= hpOutR;

		// feedback drive
		auto drive = GetParam(Parameters::driveAmount)->Value();
		if (drive > 0.0)
		{
			outL = sin(outL * drive) / drive;
			outR = sin(outR * drive) / drive;
		}

		// write to buffer
		bufferL[writePosition] = inputs[0][s] + outL;
		bufferR[writePosition] = inputs[1][s] + outR;

		// increment write position
		writePosition += 1;
		writePosition %= std::size(bufferL);

		// output
		auto dry = GetParam(Parameters::dryVolume)->Value();
		auto wet = GetParam(Parameters::wetVolume)->Value();
		outputs[0][s] = inputs[0][s] * dry + outL * wet;
		outputs[1][s] = inputs[1][s] * dry + outR * wet;
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
