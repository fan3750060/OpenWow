#pragma once

enum LightType
{
	OMNI_LGT,
	SPOT_LGT,
	DIRECT_LGT,
	AMBIENT_LGT
};

#include "WMO_Headers.h"

class WMOLight
{
public:
	WMOLight(const WMO_LightDef& _proto);

	void setup(uint32 light);

	static void setupOnce(uint32 light, vec3 dir, vec4 lcol);

public:
	vec4 fcolor;

public:
	WMO_LightDef lightDef;
};