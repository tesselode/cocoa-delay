#include "Delay.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

const int numPrograms = 128;
const int tapeLength = 10;

enum Parameters
{
	delayTime,
	lfoAmount,
	lfoFrequency,
	driftAmount,
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
	GetParam(Parameters::lfoAmount)->InitDouble("LFO amount", 0.0, 0.0, .5, .01, "", "", 2.0);
	GetParam(Parameters::lfoFrequency)->InitDouble("LFO frequency", 2.0, .1, 10.0, .01, "hz");
	GetParam(Parameters::driftAmount)->InitDouble("Drift amount", .001, 0.0, .05, .01, "", "", 2.0);
	GetParam(Parameters::tempoSync)->InitBool("Tempo sync", false);
	GetParam(Parameters::tempoSyncTime)->InitEnum("Tempo sync delay time", TempoSyncTimes::quarter, TempoSyncTimes::numTempoSyncTimes);
	GetParam(Parameters::feedback)->InitDouble("Feedback amount", 0.5, 0.0, 1.0, .01);
	GetParam(Parameters::stereoOffset)->InitDouble("Stereo offset", 0.0, -.5, .5, .01);
	GetParam(Parameters::stereoWidth)->InitDouble("Stereo width", 1.0, 0.0, 1.0, .01);
	GetParam(Parameters::pan)->InitDouble("Panning", 0.0, -pi * .5, pi * .5, .01);
	GetParam(Parameters::lowPass)->InitDouble("Low pass", .75, .01, 1.0, .01);
	GetParam(Parameters::highPass)->InitDouble("High pass", 0.0, 0.0, .99, .01);
	GetParam(Parameters::driveAmount)->InitDouble("Drive amount", 0.1, 0.0, 10.0, .01, "", "", 2.0);
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
	auto timeL = pow(baseTime, 1.0 + offset);
	auto timeR = pow(baseTime, 1.0 - offset);
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

void Delay::UpdateDrift()
{
	driftVelocity += random() * 10000.0 / GetSampleRate();
	driftVelocity -= driftVelocity * 2.0 / GetSampleRate();
	driftPhase += driftVelocity / GetSampleRate();
}

double Delay::GetDelayTime()
{
	double delayTime = 0.0;
	switch ((bool)GetParam(Parameters::tempoSync)->Value())
	{
	case true:
	{
		auto beatLength = 60 / GetTempo();
		switch ((TempoSyncTimes)(int)GetParam(Parameters::tempoSyncTime)->Value())
		{
		case whole: delayTime = beatLength * 4; break;
		case dottedHalf: delayTime = beatLength * 3; break;
		case half: delayTime = beatLength * 2; break;
		case tripletHalf: delayTime = beatLength * 4/3; break;
		case dottedQuarter: delayTime = beatLength * 3/2; break;
		case quarter: delayTime = beatLength * 1; break;
		case tripletQuarter: delayTime = beatLength * 2/3; break;
		case dottedEighth: delayTime = beatLength * 3/4; break;
		case eighth: delayTime = beatLength * 1/2; break;
		case tripletEighth: delayTime = beatLength * 1/3; break;
		case dottedSixteenth: delayTime = beatLength * 3/8; break;
		case sixteenth: delayTime = beatLength * 1/4; break;
		case tripletSixteenth: delayTime = beatLength * 1/6; break;
		case dottedThirtysecond: delayTime = beatLength * 3/16; break;
		case thirtysecond: delayTime = beatLength * 1/8; break;
		case tripletThirtysecond: delayTime = beatLength * 1/12; break;
		case dottedSixtyforth: delayTime = beatLength * 3/32; break;
		case sixtyforth: delayTime = beatLength * 1/16; break;
		case tripletSixtyforth: delayTime = beatLength * 1/24; break;
		}
	}
	case false:
		delayTime = GetParam(Parameters::delayTime)->Value(); break;
	}

	// modulation
	auto lfoAmount = GetParam(Parameters::lfoAmount)->Value();
	if (lfoAmount != 0.0) delayTime = pow(delayTime, 1.0 + lfoAmount * sin(lfoPhase * 2 * pi));
	auto driftAmount = GetParam(Parameters::driftAmount)->Value();
	if (driftAmount != 0.0) delayTime = pow(delayTime, 1.0 + driftAmount * sin(driftPhase));

	return delayTime;
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
	int p0 = wrap(floor(position) - 1, 0, std::size(buffer) - 1);
	int p1 = wrap(floor(position), 0, std::size(buffer) - 1);
	int p2 = wrap(ceil(position), 0, std::size(buffer) - 1);
	int p3 = wrap(ceil(position) + 1, 0, std::size(buffer) - 1);

	auto x = position - floor(position);
	auto y0 = buffer[p0];
	auto y1 = buffer[p1];
	auto y2 = buffer[p2];
	auto y3 = buffer[p3];

	return hermite(x, y0, y1, y2, y3);
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

		// update modulation
		lfoPhase += GetParam(Parameters::lfoFrequency)->Value() / GetSampleRate();
		while (lfoPhase > 1.0) lfoPhase -= 1.0;
		UpdateDrift();

		// read from buffer
		auto outL = GetBuffer(bufferL, writePosition - readPositionL);
		auto outR = GetBuffer(bufferR, writePosition - readPositionR);

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
		bufferL[writePosition] = inputs[0][s] + outL * GetParam(Parameters::feedback)->Value();
		bufferR[writePosition] = inputs[1][s] + outR * GetParam(Parameters::feedback)->Value();

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
