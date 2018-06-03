#include "stdafx.h"

// General
#include "M2_Part_Color.h"

// Additional
#include "WorldController.h"

CM2_Part_Color::CM2_Part_Color() :
	vColor(vec3(1.0f, 1.0f, 1.0f)),
	vAlpha(1.0f)
{
}

void CM2_Part_Color::init(IFile* f, SM2_Color& mcd, const vector<SM2_Loop>* global)
{
	color.init(mcd.color, f, global);
	alpha.init(mcd.alpha, f, global);
}

void CM2_Part_Color::calc(uint32 anim, uint32 time, uint32 globalTime)
{
	if (color.uses(anim))
	{
		vColor = color.getValue(anim, time, globalTime);
		vAlpha = alpha.getValue(anim, time, globalTime);
	}
}
