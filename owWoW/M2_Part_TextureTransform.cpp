#include "stdafx.h"

// General
#include "M2_Part_TextureTransform.h"

// Additional
#include "WorldController.h"

void CM2_Part_TextureTransform::init(IFile* f, SM2_TextureTransform& mta, const vector<SM2_Loop>* global)
{
	trans.init(mta.translation, f, global);
	roll.init(mta.rotation, f, global);
	scale.init(mta.scaling, f, global);
}

void CM2_Part_TextureTransform::calc(uint32 anim, uint32 time, uint32 globalTime)
{
	matrix = Matrix4f();
	
	if (trans.uses(anim))
	{
		tval = trans.getValue(anim, time, globalTime);
		matrix.translate(tval);
	}

	if (roll.uses(anim))
	{
		matrix.translate(vec3(0.5f, 0.5f, 0.5f));

		rval = roll.getValue(anim, time, globalTime);
		matrix.rotate(rval);

		matrix.translate(vec3(-0.5f, -0.5f, -0.5f));
	}

	if (scale.uses(anim))
	{
		sval = scale.getValue(anim, time, globalTime);
		matrix.scale(sval);
	}
}

