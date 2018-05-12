#include "Knob.h"

bool Knob::Draw(IGraphics * pGraphics)
{
	int i = mValue * mBitmap.N;
	i = BOUNDED(i, 1, mBitmap.N);
	return pGraphics->DrawBitmap(&mBitmap, &mRECT, i, &mBlend);
}
