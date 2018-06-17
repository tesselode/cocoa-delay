#include "CocoaDelay.h"
#include "IPlug_include_in_plug_src.h"
#include "IControl.h"
#include "resource.h"

void CocoaDelay::InitParameters()
{
	GetParam(Parameters::delayTime)->InitDouble("Delay time", .2, 0.001, 2.0, .01, "", "", 2.0);
	GetParam(Parameters::lfoAmount)->InitDouble("LFO amount", 0.0, 0.0, .5, .01, "", "", 2.0);
	GetParam(Parameters::lfoFrequency)->InitDouble("LFO frequency", 2.0, .1, 10.0, .01, "hz");
	GetParam(Parameters::driftAmount)->InitDouble("Drift amount", .001, 0.0, .05, .01, "", "", 2.0);
	GetParam(Parameters::tempoSyncTime)->InitEnum("Tempo sync delay time", (int)TempoSyncTimes::tempoSyncOff, (int)TempoSyncTimes::numTempoSyncTimes);
	GetParam(Parameters::feedback)->InitDouble("Feedback amount", 0.5, -1.0, 1.0, .01);
	GetParam(Parameters::stereoOffset)->InitDouble("Stereo offset", 0.0, -.5, .5, .01);
	GetParam(Parameters::panMode)->InitEnum("Pan mode", (int)PanModes::stationary, (int)PanModes::numPanModes);
	GetParam(Parameters::pan)->InitDouble("Panning", 0.0, -pi * .5, pi * .5, .01);
	GetParam(Parameters::duckAmount)->InitDouble("Ducking amount", 0.0, 0.0, 10.0, .01, "", "", 1.0);
	GetParam(Parameters::duckAttackSpeed)->InitDouble("Ducking attack", 10.0, .1, 100.0, .01, "", "", 2.0);
	GetParam(Parameters::duckReleaseSpeed)->InitDouble("Ducking release", 10.0, .1, 100.0, .01, "", "", 2.0);
	GetParam(Parameters::filterMode)->InitEnum("Filter mode", (int)FilterModes::onePole, (int)FilterModes::numFilterModes);
	GetParam(Parameters::lowCut)->InitDouble("Low pass cutoff", .75, .01, 1.0, .01);
	GetParam(Parameters::highCut)->InitDouble("High pass cutoff", 0.001, 0.001, .99, .01, "", "", 2.0);
	GetParam(Parameters::driveGain)->InitDouble("Drive amount", 0.1, 0.0, 10.0, .01, "", "", 2.0);
	GetParam(Parameters::driveMix)->InitDouble("Drive mix", 1.0, 0.0, 1.0, .01);
	GetParam(Parameters::driveCutoff)->InitDouble("Drive filter cutoff", 1.0, .01, 1.0, .01);
	GetParam(Parameters::driveIterations)->InitInt("Drive iterations", 1, 1, 16);
	GetParam(Parameters::lofiRate)->InitDouble("Lofi rate", 1.0 / 44100.0, 1.0 / 44100.0, 1.0 / 11025.0, .01, "", "", 3.0);
	GetParam(Parameters::dryVolume)->InitDouble("Dry volume", 1.0, 0.0, 2.0, .01);
	GetParam(Parameters::wetVolume)->InitDouble("Wet volume", .5, 0.0, 2.0, .01);

	// tempo sync time display text
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tempoSyncOff, "Off");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::dottedEighth, "1/8D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::dottedHalf, "1/2D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::dottedQuarter, "1/4D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::dottedSixteenth, "1/16D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::dottedSixtyforth, "1/64D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::dottedThirtysecond, "1/32D");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::eighth, "1/8");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::half, "1/2");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::quarter, "1/4");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::sixteenth, "1/16");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::sixtyforth, "1/64");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::thirtysecond, "1/32");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tripletEighth, "1/8T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tripletHalf, "1/2T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tripletQuarter, "1/4T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tripletSixteenth, "1/16T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tripletSixtyforth, "1/64T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::tripletThirtysecond, "1/32T");
	GetParam(Parameters::tempoSyncTime)->SetDisplayText((int)TempoSyncTimes::whole, "1");

	// pan mode display text
	GetParam(Parameters::panMode)->SetDisplayText((int)PanModes::stationary, "Static");
	GetParam(Parameters::panMode)->SetDisplayText((int)PanModes::circular, "Circular");
	GetParam(Parameters::panMode)->SetDisplayText((int)PanModes::pingPong, "Ping pong");

	// filter mode display text
	GetParam(Parameters::filterMode)->SetDisplayText((int)FilterModes::onePole, "1 pole");
	GetParam(Parameters::filterMode)->SetDisplayText((int)FilterModes::twoPole, "2 pole");
	GetParam(Parameters::filterMode)->SetDisplayText((int)FilterModes::fourPole, "4 pole");
	GetParam(Parameters::filterMode)->SetDisplayText((int)FilterModes::stateVariable, "State variable");
}

void CocoaDelay::InitGraphics()
{
	pGraphics = MakeGraphics(this, GUI_WIDTH, GUI_HEIGHT, 120);
	pGraphics->AttachBackground(BG_ID, BG_FN);

	auto knobLeft = pGraphics->LoadIBitmap(KNOBLEFT_ID, KNOBLEFT_FN, 53);
	auto knobMiddle = pGraphics->LoadIBitmap(KNOBMIDDLE_ID, KNOBMIDDLE_FN, 53);
	auto knobRight = pGraphics->LoadIBitmap(KNOBRIGHT_ID, KNOBRIGHT_FN, 53);
	auto tempoSyncTimesMenu = pGraphics->LoadIBitmap(TEMPOSYNCTIMESMENU_ID, TEMPOSYNCTIMESMENU_FN, (int)TempoSyncTimes::numTempoSyncTimes);
	auto panModesMenu = pGraphics->LoadIBitmap(PANMODESMENU_ID, PANMODESMENU_FN, (int)PanModes::numPanModes);
	auto filterModesMenu = pGraphics->LoadIBitmap(FILTERMODESMENU_ID, FILTERMODESMENU_FN, (int)FilterModes::numFilterModes);

	pGraphics->AttachControl(new Knob(this, 42.677 * 4, 18 * 4, (int)Parameters::delayTime, &knobLeft));
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 66.677 * 4, 26 * 4, (int)Parameters::tempoSyncTime, &tempoSyncTimesMenu));
	pGraphics->AttachControl(new Knob(this, 90.677 * 4, 18 * 4, (int)Parameters::lfoAmount, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 110.677 * 4, 18 * 4, (int)Parameters::lfoFrequency, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 138.677 * 4, 18 * 4, (int)Parameters::driftAmount, &knobLeft));

	pGraphics->AttachControl(new Knob(this, 28 * 4, 62 * 4, (int)Parameters::feedback, &knobMiddle));
	pGraphics->AttachControl(new Knob(this, 48 * 4, 62 * 4, (int)Parameters::stereoOffset, &knobMiddle));
	pGraphics->AttachControl(new Knob(this, 68 * 4, 62 * 4, (int)Parameters::pan, &knobMiddle));
	pGraphics->AttachControl(new ISwitchPopUpControl(this, 92 * 4, 70 * 4, (int)Parameters::panMode, &panModesMenu));
	pGraphics->AttachControl(new Knob(this, 116 * 4, 62 * 4, (int)Parameters::duckAmount, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 136 * 4, 62 * 4, (int)Parameters::duckAttackSpeed, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 156 * 4, 62 * 4, (int)Parameters::duckReleaseSpeed, &knobLeft));

	pGraphics->AttachControl(new ISwitchPopUpControl(this, 33 * 4, 108 * 4, (int)Parameters::filterMode, &filterModesMenu));
	pGraphics->AttachControl(new Knob(this, 48 * 4, 100 * 4, (int)Parameters::lowCut, &knobRight));
	pGraphics->AttachControl(new Knob(this, 68 * 4, 100 * 4, (int)Parameters::highCut, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 96 * 4, 100 * 4, (int)Parameters::driveGain, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 116 * 4, 100 * 4, (int)Parameters::driveMix, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 136 * 4, 100 * 4, (int)Parameters::driveCutoff, &knobRight));
	pGraphics->AttachControl(new Knob(this, 156 * 4, 100 * 4, (int)Parameters::driveIterations, &knobLeft));

	pGraphics->AttachControl(new Knob(this, 0 * 4, 76 * 4, (int)Parameters::dryVolume, &knobLeft));
	pGraphics->AttachControl(new Knob(this, 0 * 4, 100 * 4, (int)Parameters::wetVolume, &knobLeft));

	//pGraphics->AttachControl(new PresetMenu(this, IRECT(0, 0, 100, 25)));

	AttachGraphics(pGraphics);
}

CocoaDelay::CocoaDelay(IPlugInstanceInfo instanceInfo)
	: IPLUG_CTOR((int)Parameters::numParameters, numPrograms, instanceInfo)
{
	TRACE;

	InitParameters();
	InitGraphics();
	InitPresets();
}

CocoaDelay::~CocoaDelay() {}

double CocoaDelay::GetDelayTime()
{
	double delayTime = 0.0;
	auto beatLength = 60 / GetTempo();
	switch ((TempoSyncTimes)(int)GetParam(Parameters::tempoSyncTime)->Value())
	{
	case TempoSyncTimes::tempoSyncOff:
		delayTime = GetParam(Parameters::delayTime)->Value();
		break;
	case TempoSyncTimes::whole:               delayTime = beatLength * 4;    break;
	case TempoSyncTimes::dottedHalf:          delayTime = beatLength * 3;    break;
	case TempoSyncTimes::half:                delayTime = beatLength * 2;    break;
	case TempoSyncTimes::tripletHalf:         delayTime = beatLength * 4/3;  break;
	case TempoSyncTimes::dottedQuarter:       delayTime = beatLength * 3/2;  break;
	case TempoSyncTimes::quarter:             delayTime = beatLength * 1;    break;
	case TempoSyncTimes::tripletQuarter:      delayTime = beatLength * 2/3;  break;
	case TempoSyncTimes::dottedEighth:        delayTime = beatLength * 3/4;  break;
	case TempoSyncTimes::eighth:              delayTime = beatLength * 1/2;  break;
	case TempoSyncTimes::tripletEighth:       delayTime = beatLength * 1/3;  break;
	case TempoSyncTimes::dottedSixteenth:     delayTime = beatLength * 3/8;  break;
	case TempoSyncTimes::sixteenth:           delayTime = beatLength * 1/4;  break;
	case TempoSyncTimes::tripletSixteenth:    delayTime = beatLength * 1/6;  break;
	case TempoSyncTimes::dottedThirtysecond:  delayTime = beatLength * 3/16; break;
	case TempoSyncTimes::thirtysecond:        delayTime = beatLength * 1/8;  break;
	case TempoSyncTimes::tripletThirtysecond: delayTime = beatLength * 1/12; break;
	case TempoSyncTimes::dottedSixtyforth:    delayTime = beatLength * 3/32; break;
	case TempoSyncTimes::sixtyforth:          delayTime = beatLength * 1/16; break;
	case TempoSyncTimes::tripletSixtyforth:   delayTime = beatLength * 1/24; break;
	}

	// modulation
	auto lfoAmount = GetParam(Parameters::lfoAmount)->Value();
	if (lfoAmount != 0.0) delayTime = pow(delayTime, 1.0 + lfoAmount * sin(lfoPhase * 2 * pi));
	auto driftAmount = GetParam(Parameters::driftAmount)->Value();
	if (driftAmount != 0.0) delayTime = pow(delayTime, 1.0 + driftAmount * sin(driftPhase));

	return delayTime;
}

void CocoaDelay::GetReadPositions(double &l, double &r)
{
	auto offset = GetParam(Parameters::stereoOffset)->Value() * .5;
	auto baseTime = GetDelayTime();
	auto timeL = pow(baseTime, 1.0 + offset);
	auto timeR = pow(baseTime, 1.0 - offset);
	l = timeL * GetSampleRate();
	r = timeR * GetSampleRate();
}

void CocoaDelay::InitBuffer()
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

void CocoaDelay::UpdateReadPositions()
{
	double targetReadPositionL, targetReadPositionR;
	GetReadPositions(targetReadPositionL, targetReadPositionR);
	readPositionL += (targetReadPositionL - readPositionL) * 10.0 * dt;
	readPositionR += (targetReadPositionR - readPositionR) * 10.0 * dt;
}

void CocoaDelay::UpdateWritePosition()
{
	writePosition += 1;
	writePosition %= std::size(bufferL);
}

void CocoaDelay::UpdateParameters()
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

void CocoaDelay::UpdateDucking(double input)
{
	auto attackSpeed = GetParam(Parameters::duckAttackSpeed)->Value();
	auto releaseSpeed = GetParam(Parameters::duckReleaseSpeed)->Value();
	auto speed = duckFollower < abs(input) ? attackSpeed : releaseSpeed;
	duckFollower += (abs(input) - duckFollower) * speed * dt;
}

void CocoaDelay::UpdateLfo()
{
	lfoPhase += GetParam(Parameters::lfoFrequency)->Value() * dt;
	while (lfoPhase > 1.0) lfoPhase -= 1.0;
}

void CocoaDelay::UpdateDrift()
{
	driftVelocity += random() * 10000.0 * dt;
	driftVelocity -= driftVelocity * 2.0 * dt;
	driftPhase += driftVelocity * dt;
}

double CocoaDelay::GetSample(std::vector<double> &buffer, double position)
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

void CocoaDelay::WriteToBuffer(double** inputs, int s, double outL, double outR)
{
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
}

void CocoaDelay::ProcessDoubleReplacing(double** inputs, double** outputs, int nFrames)
{
	for (int s = 0; s < nFrames; s++)
	{
		UpdateParameters();
		UpdateReadPositions();
		UpdateDucking(inputs[0][s] + inputs[1][s]);
		UpdateLfo();
		UpdateDrift();

		// read from buffer
		auto outL = GetSample(bufferL, writePosition - readPositionL);
		auto outR = GetSample(bufferR, writePosition - readPositionR);

		// circular panning
		adjustPanning(outL, outR, circularPanAmount, outL, outR);

		// filters
		lp.Process(dt, outL, outR, GetParam(Parameters::lowCut)->Value());
		hp.Process(dt, outL, outR, GetParam(Parameters::highCut)->Value(), true);

		// drive
		auto driveAmount = GetParam(Parameters::driveGain)->Value();
		auto driveMix = GetParam(Parameters::driveMix)->Value();
		if (driveAmount > 0)
		{
			auto iterations = (int)GetParam(Parameters::driveIterations)->Value();
			for (int i = 0; i < iterations; i++)
			{
				outL = statefulDrive.Process(outL * driveAmount, driveMix) / driveAmount;
				outR = statefulDrive.Process(outR * driveAmount, driveMix) / driveAmount;
				driveFilter.Process(dt, outL, outR, GetParam(Parameters::driveCutoff)->Value(), outL, outR);
			}
		}

		// lofi
		if (GetParam(Parameters::lofiRate)->Value() != GetParam(Parameters::lofiRate)->GetMin())
		{
			outL = lofiL.Process(dt, outL, GetParam(Parameters::lofiRate)->Value());
			outR = lofiR.Process(dt, outR, GetParam(Parameters::lofiRate)->Value());
		}

		// write to buffer
		WriteToBuffer(inputs, s, outL, outR);
		UpdateWritePosition();

		// output
		auto dry = GetParam(Parameters::dryVolume)->Value();
		auto wet = GetParam(Parameters::wetVolume)->Value();
		auto duckValue = GetParam(Parameters::duckAmount)->Value() * duckFollower;
		duckValue = duckValue > 1.0 ? 1.0 : duckValue;
		wet *= 1.0 - duckValue;
		outputs[0][s] = inputs[0][s] * dry + outL * wet;
		outputs[1][s] = inputs[1][s] * dry + outR * wet;
	}
}

void CocoaDelay::Reset()
{
	TRACE;
	IMutexLock lock(this);
	dt = 1.0 / GetSampleRate();
	InitBuffer();
}

void CocoaDelay::OnParamChange(int paramIdx)
{
	IMutexLock lock(this);

	switch ((Parameters)paramIdx)
	{
	case Parameters::filterMode:
		lp.SetMode((FilterModes)(int)GetParam(Parameters::filterMode)->Value());
		hp.SetMode((FilterModes)(int)GetParam(Parameters::filterMode)->Value());
		break;
	case Parameters::tempoSyncTime:
	{
		auto tempoSyncTime = (TempoSyncTimes)(int)GetParam(Parameters::tempoSyncTime)->Value();
		pGraphics->GetControl(1)->GrayOut(tempoSyncTime != TempoSyncTimes::tempoSyncOff);
		break;
	}
	case Parameters::lfoAmount:
		pGraphics->GetControl(4)->GrayOut(GetParam(Parameters::lfoAmount)->Value() == 0.0);
		break;
	case Parameters::duckAmount:
	{
		auto duckingEnabled = GetParam(Parameters::duckAmount)->Value() > 0.0;
		pGraphics->GetControl(11)->GrayOut(!duckingEnabled);
		pGraphics->GetControl(12)->GrayOut(!duckingEnabled);
		break;
	}
	case Parameters::driveGain:
	{
		auto driveEnabled = GetParam(Parameters::driveGain)->Value() > 0.0;
		pGraphics->GetControl(17)->GrayOut(!driveEnabled);
		pGraphics->GetControl(18)->GrayOut(!driveEnabled);
		break;
	}
	}
}
