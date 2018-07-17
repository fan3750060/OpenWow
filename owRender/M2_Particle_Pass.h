#pragma once

#include "GeometryBase.h"

class CM2_Particle_Pass : public GeometryBase
{
public:
	CM2_Particle_Pass(RenderDevice* _RenderDevice) :
		GeometryBase(_RenderDevice, "shaders/M2/M2_Particles")
	{
		gColorMap = getLocation("gColorMap");
		gColor = getLocation("gColor");
	}

	inline void SetColorTextureUnit(int TextureUnit)
	{
		setTexture(gColorMap, TextureUnit);
	}

	inline void SetColor(vec4 _color)
	{
		setVec4(gColor, _color);
	}

private:
	int8 gColorMap;
	int8 gColor;
};