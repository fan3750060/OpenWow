#pragma once

#include "technique.h"

class MapTileLowRes_GeometryPass : public Technique
{
public:
	MapTileLowRes_GeometryPass() : Technique("shaders/MapTileLowRes") 
    {
        gColor = getLocation("gColor");
    }

	void SetShadowColor(vec3 _Color)
	{
		setVec3(gColor, _Color);
	}

private:
    int32 gColor;
};