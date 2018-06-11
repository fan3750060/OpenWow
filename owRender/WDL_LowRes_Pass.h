#pragma once

#include "technique.h"

class WDL_LowRes_Pass : public Technique
{
public:
	WDL_LowRes_Pass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/Map/MapTileLowRes")
    {
        gColor = getLocation("gColor");
    }

	//--

	void SetShadowColor(vec3 _Color)
	{
		setVec3(gColor, _Color);
	}

private:
    int32 gColor;
};