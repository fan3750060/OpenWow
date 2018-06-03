#include "stdafx.h"

// General
#include "M2_Part_TextureWeight.h"

// Additional
#include "WorldController.h"

void CM2_Part_TextureWeight::init(IFile* f, SM2_TextureWeight& mtd, const vector<SM2_Loop>* global)
{
	trans.init(mtd.weight, f, global);

	tVal = 1.0f;
}

void CM2_Part_TextureWeight::calc(uint32 anim, uint32 time, uint32 globalTime)
{
	if (trans.uses(anim))
	{
		tVal = trans.getValue(anim, time, globalTime);
	}
}