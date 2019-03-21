#pragma once

#include "GeometryBase.h"

class CWDL_LowRes_Pass : public GeometryBase
{
public:
	CWDL_LowRes_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/Map/MapTileLowRes")
    {
        gColor = getLocation("gColor");
    }

	void setColor(vec3 _Color)
	{
		setVec3(gColor, _Color);
	}

private:
    int32 gColor;
};