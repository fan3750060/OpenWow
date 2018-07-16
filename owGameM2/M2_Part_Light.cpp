#include "stdafx.h"

// Include
#include "M2.h"

// General
#include "M2_Part_Light.h"

CM2_Part_Light::CM2_Part_Light(const M2* _parentM2, IFile* f, const SM2_Light& _proto, cGlobalLoopSeq global) :
	m_Bone(nullptr),

	ambColorValue(vec3(1.0f, 1.0f, 1.0f)),
	ambIntensityValue(1.0f),
	diffColorValue(vec3(1.0f, 1.0f, 1.0f)),
	diffIntensityValue(1.0f),
	attenuation_startValue(0.0f),
	attenuation_endValue(1.0f),
	visibilityValue(false)
{
	type = _proto.type;
	if (_proto.bone != -1)
	{
		m_Bone = _parentM2->getSkeleton()->getBoneLookup(_proto.bone);
	}

	position = _proto.position.toXZmY();

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

	if (m_Bone != nullptr)
	{
		if (type == SM2_Light::Type::Directional)
		{
			positionValue = m_Bone->getTransformMatrix() * position;
		}
		else if (type == SM2_Light::Type::Point)
		{
			directionValue = m_Bone->getRotateMatrix() * direction;
		}		
	}

	//Log::Info("Light %d (%f,%f,%f) (%f,%f,%f) [%f,%f,%f]", l-GL_LIGHT4, ambcol.x, ambcol.y, ambcol.z, diffcol.x, diffcol.y, diffcol.z, p.x, p.y, p.z);
	//glLightfv(l, GL_POSITION, p);
	//glLightfv(l, GL_DIFFUSE, diffcol);
	//glLightfv(l, GL_AMBIENT, ambcol);
	//glEnable(l);
}