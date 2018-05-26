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
	tempoSyncTime,
	feedback,
	stereoOffset,
	panMode,
	pan,
	lpMode,
	lpCut,
	highPass,
	driveAmount,
	dryVolume,
	wetVolume,
	numParameters
};

enum TempoSyncTimes
{
	tempoSyncOff,
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

enum FilterModes
{
	onePole,
	stateVariable,
	numFilterModes
};

void Delay::InitParameters()
{
	GetParam(Parameters::delayTime)->InitDouble("Delay time", .2, 0.001, 2.0, .01, "", "", 2.0);
	GetParam(Parameters::lfoAmount)->InitDouble("LFO amount", 0.0, 0.0, .5, .01, "", "", 2.0);
	GetParam(Parameters::lfoFrequency)->InitDouble("LFO frequency", 2.0, .1, 10.0, .01, "hz");
	GetParam(Parameters::driftAmount)->InitDouble("Drift amount", .001, 0.0, .05, .01, "", "", 2.0);
	GetParam(Parameters::tempoSyncTime)->InitEnum("Tempo sync delay time", TempoSyncTimes::tempoSyncOff, TempoSyncTimes::numTempoSyncTimes);
	GetParam(Parameters::feedback)->InitDouble("Feedback amount", 0.5, 0.0, 1.0, .01);
	GetParam(Parameters::stereoOffset)->InitDouble("Stereo offset", 0.0, -.5, .5, .01);
	GetParam(Parameters::panMode)->InitEnum("Pan mode", PanModes::stationary, PanModes::numPanModes);
	GetParam(Parameters::pan)->InitDouble("Panning", 0.0, -pi * .5, pi * .5, .01);
	GetParam(Parameters::lpMode)->InitEnum("Low pass mode", FilterModes::onePole, FilterModes::numFilterModes);
	GetParam(Parameters::lpCut)->InitDouble("Low pass cutoff", .75, .01, 1.0, .01);
	GetParam(Parameters::highPass)->InitDouble("High pass", 0.0, 0.0, .99, .01, "", "", 2.0);
	GetParam(Parameters::driveAmount)->InitDouble("Drive amount", 0.1, 0.0, 10.0, .01, "", "", 2.0);
	GetParam(Parameters::dryVolume)->InitDouble("Dry volume", 1.0, 0.0, 2.0, .01);
	GetParam(Parameters::wetVolume)->InitDouble("Wet volume", .5, 0.0, 2.0, .01);

	// tempo sync time display text
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tempoSyncOff, "Off");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::dottedEighth, "1/8D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::dottedHalf, "1/2D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::dottedQuarter, "1/4D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::dottedSixteenth, "1/16D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::dottedSixtyforth, "1/64D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::dottedThirtysecond, "1/32D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::eighth, "1/8");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::half, "1/2");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::quarter, "1/4");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::sixteenth, "1/16");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::sixtyforth, "1/64");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::thirtysecond, "1/32");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tripletEighth, "1/8T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tripletHalf, "1/2T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tripletQuarter, "1/4T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tripletSixteenth, "1/16T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tripletSixtyforth, "1/64T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::tripletThirtysecond, "1/32T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText(TempoSyncTimes::whole, "1");

	// pan mode display text
	GetParam(Parameters::panMode)->SetDisplayText(PanModes::stationary, "Static");
	GetParam(Parameters::panMode)->SetDisplayText(PanModes::circular, "Circular");
	GetParam(Parameters::panMode)->SetDisplayText(PanModes::pingPong, "Ping pong");
}

void Delay::InitGraphics()
{
	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 53);
	auto knobMiddle = pGraphics->LoadIBitmap(KNOBMIDDLE_ID, KNOBMIDDLE_FN, 53);
	auto knobRight = pGraphics->LoadIBitmap(KNOBRIGHT_ID, KNOBRIGHT_FN, 53);
	auto tempoSyncTimesMenu = pGraphics->LoadIBitmap(TEMPOSYNCTIMESMENU_ID, TEMPOSYNCTIMESMENU_FN, numTempoSyncTimes);
	auto panModesMenu = pGraphics->LoadIBitmap(PANMODESMENU_ID, PANMODESMENU_FN, numPanModes);

	pGraphics->AttachControl(new Knob(this, 28 * 4, 17 * 4, Parameters::delayTime, &knobLeft));
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 52 * 4, 25 * 4, Parameters::tempoSyncTime, &tempoSyncTimesMenu));
	pGraphics->AttachControl(new Knob(this, 128 * 4, 17 * 4, Parameters::feedback, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 148 * 4, 18 * 4, Parameters::stereoOffset, &knobMiddle));
	pGraphics->AttachControl(new Knob(this, 168 * 4, 18 * 4, Parameters::pan, &knobMiddle));
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 192 * 4, 25 * 4, Parameters::panMode, &panModesMenu));
	pGraphics->AttachControl(new Knob(this, 28 * 4, 56 * 4, Parameters::lfoAmount, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 48 * 4, 56 * 4, Parameters::lfoFrequency, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 86 * 4, 56 * 4, Parameters::driftAmount, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 132.5 * 4, 56 * 4, Parameters::lpCut, &knobRight));
	pGraphics->AttachControl(new Knob(this, 152.5 * 4, 56 * 4, Parameters::highPass, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 180.5 * 4, 56 * 4, Parameters::driveAmount, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 0 * 4, 32 * 4, Parameters::dryVolume, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 0 * 4, 56 * 4, Parameters::wetVolume, &knobLeft));

	AttachGraphics(pGraphics);
}

void Delay::InitPresets()
{
	MakeDefaultPreset("-", numPrograms);
}

Delay::Delay(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR(numParameters, numPrograms, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
}

Delay::~Delay() {}

double Delay::GetDelayTime()
{
	double delayTime = 0.0;
	auto beatLength = 60 / GetTempo();
	switch ((TempoSyncTimes)(int)GetParam(Parameters::tempoSyncTime)->Value())
	{
	case tempoSyncOff:
		delayTime = GetParam(Parameters::delayTime)->Value();
		break;
	case whole: delayTime = beatLength * 4; break;
	case dottedHalf: delayTime = beatLength * 3; break;
	case half: delayTime = beatLength * 2; break;
	case tripletHalf: delayTime = beatLength * 4 / 3; break;
	case dottedQuarter: delayTime = beatLength * 3 / 2; break;
	case quarter: delayTime = beatLength * 1; break;
	case tripletQuarter: delayTime = beatLength * 2 / 3; break;
	case dottedEighth: delayTime = beatLength * 3 / 4; break;
	case eighth: delayTime = beatLength * 1 / 2; break;
	case tripletEighth: delayTime = beatLength * 1 / 3; break;
	case dottedSixteenth: delayTime = beatLength * 3 / 8; break;
	case sixteenth: delayTime = beatLength * 1 / 4; break;
	case tripletSixteenth: delayTime = beatLength * 1 / 6; break;
	case dottedThirtysecond: delayTime = beatLength * 3 / 16; break;
	case thirtysecond: delayTime = beatLength * 1 / 8; break;
	case tripletThirtysecond: delayTime = beatLength * 1 / 12; break;
	case dottedSixtyforth: delayTime = beatLength * 3 / 32; break;
	case sixtyforth: delayTime = beatLength * 1 / 16; break;
	case tripletSixtyforth: delayTime = beatLength * 1 / 24; break;
	}

	// modulation
	auto lfoAmount = GetParam(Parameters::lfoAmount)->Value();
	if (lfoAmount != 0.0) delayTime = pow(delayTime, 1.0 + lfoAmount * sin(lfoPhase * 2 * pi));
	auto driftAmount = GetParam(Parameters::driftAmount)->Value();
	if (driftAmount != 0.0) delayTime = pow(delayTime, 1.0 + driftAmount * sin(driftPhase));

	return delayTime;
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

void Delay::UpdateReadPositions()
{
	double targetReadPositionL, targetReadPositionR;
	GetReadPositions(targetReadPositionL, targetReadPositionR);
	readPositionL += (targetReadPositionL - readPositionL) * 10.0 * dt;
	readPositionR += (targetReadPositionR - readPositionR) * 10.0 * dt;
}

void Delay::UpdateWritePosition()
{
	writePosition += 1;
	writePosition %= std::size(bufferL);
}

void Delay::UpdateParameters()
{
	// pan mode fadeout
	if (currentPanMode != (PanModes)(int)GetParam(Parameters::panMode)->Value())
	{
		parameterChangeVolume -= 100.0 * dt;
		if (parameterChangeVolume <= 0.0)
		{
			parameterChangeVolume = 0.0;
			currentPanMode = (PanModes)(int)GetParam(Parameters::panMode)->Value();
		}
	}
	else if (parameterChangeVolume < 1.0)
	{
		parameterChangeVolume += 100.0 * dt;
		if (parameterChangeVolume > 1.0) parameterChangeVolume = 1.0;
	}

	// pan amount smoothing
	auto panAmount = GetParam(Parameters::pan)->Value();
	auto stationaryPanAmountTarget = (currentPanMode == PanModes::stationary || currentPanMode == PanModes::pingPong) ? panAmount : 0.0;
	stationaryPanAmount += (stationaryPanAmountTarget - stationaryPanAmount) * 100.0 * dt;
	auto circularPanAmountTarget = (currentPanMode == PanModes::circular ? panAmount : 0.0);
	circularPanAmount += (circularPanAmountTarget - circularPanAmount) * 100.0 * dt;
}

void Delay::UpdateLfo()
{
	lfoPhase += GetParam(Parameters::lfoFrequency)->Value() * dt;
	while (lfoPhase > 1.0) lfoPhase -= 1.0;
}

void Delay::UpdateDrift()
{
	driftVelocity += random() * 10000.0 * dt;
	driftVelocity -= driftVelocity * 2.0 * dt;
	driftPhase += driftVelocity * dt;
}

double Delay::GetSample(std::vector<double> &buffer, double position)
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

	return interpolate(x, y0, y1, y2, y3);
}

void Delay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		UpdateParameters();
		UpdateReadPositions();
		UpdateLfo();
		UpdateDrift();

		// read from buffer
		auto outL = GetSample(bufferL, writePosition - readPositionL);
		auto outR = GetSample(bufferR, writePosition - readPositionR);

		// circular panning
		adjustPanning(outL, outR, circularPanAmount, outL, outR);

		// filters
		switch ((FilterModes)(int)GetParam(Parameters::lpMode)->Value())
		{
		case FilterModes::onePole:
			outL = lpL.Process(dt, outL, GetParam(Parameters::lpCut)->Value());
			outR = lpR.Process(dt, outR, GetParam(Parameters::lpCut)->Value());
			break;
		case FilterModes::stateVariable:
			outL = svfL.Process(dt, outL, GetParam(Parameters::lpCut)->Value());
			outR = svfR.Process(dt, outR, GetParam(Parameters::lpCut)->Value());
			break;
		}
		outL -= hpL.Process(dt, outL, GetParam(Parameters::highPass)->Value());
		outR -= hpR.Process(dt, outR, GetParam(Parameters::highPass)->Value());

		// drive
		auto driveAmount = GetParam(Parameters::driveAmount)->Value();
		if (driveAmount > 0)
		{
			outL = statefulDrive.Process(outL * driveAmount) / driveAmount;
			outR = statefulDrive.Process(outR * driveAmount) / driveAmount;
		}

		// write to buffer
		auto writeL = 0.0, writeR = 0.0;
		writeL += inputs[0][s];
		writeR += inputs[1][s];
		adjustPanning(writeL, writeR, stationaryPanAmount * .5, writeL, writeR);
		writeL += outL * GetParam(Parameters::feedback)->Value();
		writeR += outR * GetParam(Parameters::feedback)->Value();
		switch (currentPanMode)
		{
		case PanModes::pingPong:
			bufferL[writePosition] = writeR * parameterChangeVolume;
			bufferR[writePosition] = writeL * parameterChangeVolume;
			break;
		default:
			bufferL[writePosition] = writeL * parameterChangeVolume;
			bufferR[writePosition] = writeR * parameterChangeVolume;
			break;
		}
		UpdateWritePosition();

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
	dt = 1.0 / GetSampleRate();
	InitBuffer();
}

void Delay::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch (paramIdx)
	{
	case Parameters::tempoSyncTime:
	{
		auto tempoSyncTime = (TempoSyncTimes)(int)GetParam(Parameters::tempoSyncTime)->Value();
		//pGraphics->GetControl(1)->GrayOut(tempoSyncTime != TempoSyncTimes::tempoSyncOff);
		break;
	}
	case Parameters::lfoAmount:
		//pGraphics->GetControl(10)->GrayOut(GetParam(Parameters::lfoAmount)->Value() == 0.0);
		break;
	}
}
