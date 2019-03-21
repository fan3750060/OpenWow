#pragma once

#include "GeometryBase.h"

class CMagma_Pass : public GeometryBase
{
public:
	CMagma_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/Liquid/Magma")
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