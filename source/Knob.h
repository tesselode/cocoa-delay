#pragma once

#include "IControl.h"

class Knob : public IKnobMultiControl
{
public:
	Knob(IPlugBase* pPlug, int x, int y, int paramIdx, IBitmap* pBitmap,
		EDirection direction = kVertical, double gearing = DEFAULT_GEARING) :
		IKnobMultiControl(pPlug, x, y, paramIdx, pBitmap, direction, gearing) {}
	bool Draw(IGraphics* pGraphics);
};

