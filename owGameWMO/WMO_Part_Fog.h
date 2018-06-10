#pragma once

#include "WMO_Headers.h"

class WMO_Part_Fog : public CRefItem
{
public:
	WMO_Part_Fog(const WMO_FogDef& _proto);

	void setup();

public:
	vec4 color;

public:
	WMO_FogDef fogDef;
};