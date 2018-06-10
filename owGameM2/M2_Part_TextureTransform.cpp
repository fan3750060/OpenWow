#include "stdafx.h"

// General
#include "M2_Part_TextureTransform.h"

CM2_Part_TextureTransform::CM2_Part_TextureTransform(IFile* f, const SM2_TextureTransform& _proto, cGlobalLoopSeq global)
{
	trans.init(_proto.translation, f, global);
	roll.init(_proto.rotation, f, global);
	scale.init(_proto.scaling, f, global);
}

void CM2_Part_TextureTransform::calc(uint16 anim, uint32 time, uint32 globalTime)
{
	matrix = Matrix4f();
	
	if (trans.uses(anim))
	{
		transValue = trans.getValue(anim, time, globalTime);
		matrix.translate(transValue);
	}

	if (roll.uses(anim))
	{
		matrix.translate(vec3(0.5f, 0.5f, 0.5f));

		rollValue = roll.getValue(anim, time, globalTime);
		matrix.rotate(rollValue);

		matrix.translate(vec3(-0.5f, -0.5f, -0.5f));
	}

	if (scale.uses(anim))
	{
		scaleVal = scale.getValue(anim, time, globalTime);
		matrix.scale(scaleVal);
	}
}

