#pragma once

#include "technique.h"

class RibbonEmitters_Pass : public Technique
{
public:
	RibbonEmitters_Pass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/RibbonEmitters")
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