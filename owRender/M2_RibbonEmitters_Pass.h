#pragma once

#include "GeometryBase.h"

class CM2_RibbonEmitters_Pass : public GeometryBase
{
public:
	CM2_RibbonEmitters_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/M2/M2_RibbonEmitters")
	{
		gColorMap = getLocation("gColorMap");
	}

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture(gColorMap, TextureUnit);
	}

private:
	int8 gColorMap;
};