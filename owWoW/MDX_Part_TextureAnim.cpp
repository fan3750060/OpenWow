#include "stdafx.h"

// General
#include "MDX_Part_TextureAnim.h"

// Additional
#include "WorldController.h"

void MDX_Part_TextureAnim::init(File& f, M2TextureTransform& mta, uint32 * global)
{
	trans.init(mta.translation, f, global);
	roll.init(mta.rotation, f, global);
	scale.init(mta.scaling, f, global);
}

void MDX_Part_TextureAnim::calc(int anim, int time)
{
	matrix = Matrix4f();
	
	if (trans.uses(anim))
	{
		tval = trans.getValue(anim, time, _World->EnvM()->globalTime);
		matrix.translate(tval);
	}

	if (roll.uses(anim))
	{
		matrix.translate(vec3(0.5f, 0.5f, 0.5f));

		rval = roll.getValue(anim, time, _World->EnvM()->globalTime);
		matrix.rotate(rval);

		matrix.translate(vec3(-0.5f, -0.5f, -0.5f));
	}

	if (scale.uses(anim))
	{
		sval = scale.getValue(anim, time, _World->EnvM()->globalTime);
		matrix.scale(sval);
	}
}

