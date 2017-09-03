#include "stdafx.h"

// General
#include "Model_TextureAnim.h"


void ModelTextureAnim::init(File& f, ModelTexAnimDef& mta, uint32_t * global)
{
	trans.init(mta.trans, f, global);
	rot.init(mta.rot, f, global);
	scale.init(mta.scale, f, global);
}

void ModelTextureAnim::calc(int anim, int time)
{
	if (trans.uses(anim))
	{
		tval = trans.getValue(anim, time);
	}

	if (rot.uses(anim))
	{
		rval = rot.getValue(anim, time);
	}

	if (scale.uses(anim))
	{
		sval = scale.getValue(anim, time);
	}
}

void ModelTextureAnim::setup(int anim)
{
	glLoadIdentity();

	if (trans.uses(anim))
	{
		glTranslatef(tval.x, tval.y, tval.z);
	}

	if (rot.uses(anim))
	{
		glRotatef(rval.x, 0, 0, 1.0f); // this is wrong, I have no idea what I'm doing here ;)
	}

	if (scale.uses(anim))
	{
		glScalef(sval.x, sval.y, sval.z);
	}
}
