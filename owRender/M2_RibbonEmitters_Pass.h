#pragma once

#include "technique.h"

class M2_RibbonEmitters_Pass : public Technique
{
public:
	M2_RibbonEmitters_Pass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/M2/M2_RibbonEmitters")
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