#include "stdafx.h"

// General
#include "M2_Part_Light.h"

CM2_Part_Light::CM2_Part_Light(IFile* f, const SM2_Light& _proto, cGlobalLoopSeq global)
{
	type = _proto.type;
	bone = _proto.bone;
	position = _proto.position.toXZmY();

	direction = vec3(0, 1, 0);
	
	

	ambColor.init(_proto.ambient_color, f, global);
	ambIntensity.init(_proto.ambient_intensity, f, global);

	diffColor.init(_proto.diffuse_color, f, global);
	diffIntensity.init(_proto.diffuse_intensity, f, global);

	attenuation_start.init(_proto.attenuation_start, f, global);
	attenuation_end.init(_proto.attenuation_end, f, global);

	visibility.init(_proto.visibility, f, global);
}

void CM2_Part_Light::setup(uint16 anim, uint32 time, uint32 globalTime)
{
	if (ambColor.uses(anim))
	{
		ambColorValue = ambColor.getValue(0, time, globalTime);
	}
	if (ambIntensity.uses(anim))
	{
		ambIntensityValue = ambIntensity.getValue(0, time, globalTime);
	}

	if (diffColor.uses(anim))
	{
		diffColorValue = diffColor.getValue(0, time, globalTime);
	}
	if (diffIntensity.uses(anim))
	{
		diffIntensityValue = diffIntensity.getValue(0, time, globalTime);
	}

	vec4 ambcol(ambColorValue * ambIntensityValue, 1.0f);
	vec4 diffcol(diffColorValue * diffIntensityValue, 1.0f);
	vec4 p;

	if (type == MODELLIGHT_DIRECTIONAL)
	{
		// directional
		p = vec4(direction, 0.0f);
	}
	else if (type == MODELLIGHT_POINT)
	{
		// point
		p = vec4(position, 1.0f);
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