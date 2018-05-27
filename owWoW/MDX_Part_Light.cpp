#include "stdafx.h"

// General
#include "MDX_Part_Light.h"

// Additional
#include "WorldController.h"

void MDX_Part_Light::init(IFile* f, M2Light& mld, uint32* global)
{
	tpos = pos = mld.position.toXZmY();
	tdir = dir = vec3(0, 1, 0); // no idea
	type = mld.type;
	parent = mld.bone;

	ambColor.init(mld.ambient_color, f, global);
	ambIntensity.init(mld.ambient_intensity, f, global);
	diffColor.init(mld.diffuse_color, f, global);
	diffIntensity.init(mld.diffuse_intensity, f, global);
}

void MDX_Part_Light::setup(int time, uint32 l)
{
	vec4 ambcol(ambColor.getValue(0, time, _World->EnvM()->globalTime) * ambIntensity.getValue(0, time, _World->EnvM()->globalTime), 1.0f);
	vec4 diffcol(diffColor.getValue(0, time, _World->EnvM()->globalTime) * diffIntensity.getValue(0, time, _World->EnvM()->globalTime), 1.0f);
	vec4 p;

	if (type == MODELLIGHT_DIRECTIONAL)
	{
		// directional
		p = vec4(tdir, 0.0f);
	}
	else if (type == MODELLIGHT_POINT)
	{
		// point
		p = vec4(tpos, 1.0f);
	}
	else
	{
		Log::Error("ModelLight[]: Error: Light type [%d] is unknown.", type);
		return;
	}

	//Log::Info("Light %d (%f,%f,%f) (%f,%f,%f) [%f,%f,%f]", l-GL_LIGHT4, ambcol.x, ambcol.y, ambcol.z, diffcol.x, diffcol.y, diffcol.z, p.x, p.y, p.z);
	//glLightfv(l, GL_POSITION, p);
	//glLightfv(l, GL_DIFFUSE, diffcol);
	//glLightfv(l, GL_AMBIENT, ambcol);
	//glEnable(l);
}