#include "Knob.h"

bool Knob::Draw(IGraphics * pGraphics)
{
	int i = (int)(mValue * (mBitmap.N - 1));
	i = BOUNDED(i, 1, mBitmap.N);
	return pGraphics->DrawBitmap(&mBitmap, &mRECT, i, &mBlend);
}
