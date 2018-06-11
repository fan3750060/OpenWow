#pragma once

#include "technique.h"

class Magma_Pass : public Technique
{
public:
	Magma_Pass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/Liquid/Magma")
	{
		gColorMap = getLocation("gColorMap");
	}

	//

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture(gColorMap, TextureUnit);
	}

private:
	int32 gColorMap;
};