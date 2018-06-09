#pragma once

#include "WMO_Headers.h"

class WMOFog
{
public:
	WMOFog(IFile* f);

	void setup();

public:
	vec4 color;

public:
	WMO_FogDef fogDef;
};