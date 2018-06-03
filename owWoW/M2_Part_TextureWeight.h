#pragma once

#include "M2_Types.h"

struct CM2_Part_TextureWeight
{
	friend class MDX;
	friend class MDX_Skin_Batch;
public:
	void init(IFile* f, SM2_TextureWeight& mtd, const vector<SM2_Loop>* global);
	void calc(uint32 anim, uint32 time, uint32 globalTime);
	float getValue() { return tVal; }

private:
	M2_Animated<float, short, ShortToFloat> trans;
	float tVal;
};