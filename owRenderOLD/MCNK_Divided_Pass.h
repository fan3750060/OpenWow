#pragma once

#include "GeometryBase.h"

class CMCNK_Divided_Pass : public GeometryBase
{
public:
	CMCNK_Divided_Pass(RenderDevice* _RenderDevice, uint8 _num) :
		GeometryBase(_RenderDevice, "shaders/Map/MapChunk.vs", "shaders/Map/MapChunk_WotLK_" + to_string(_num) + ".fs")
    {
        for (uint8 i = 0; i < _num + 1; i++)
        {
            gColorMap[i] = getLocation((string("gColorMap[") + to_string(i) + "]").c_str());
            gSpecularMap[i] = getLocation((string("gSpecularMap[") + to_string(i) + "]").c_str());
        }

        gBlend = getLocation("gBlend");

        gShadowMapExists = getLocation("gShadowMapExists");
        gShadowColor = getLocation("gShadowColor");
    }

	//--

	void SetColorTextureUnit(int _number)
	{
		setTexture(gColorMap[_number], CMCNK_Divided_Pass::C_ColorsStart + _number);
	}
    void SetSpecularTextureUnit(int _number)
    {
        setTexture(gSpecularMap[_number], CMCNK_Divided_Pass::C_SpecularStart + _number);
    }

	void SetBlendBuffer(int TextureUnit)
	{
		setTexture(gBlend, TextureUnit);
	}

	void SetShadowMapExists(bool _exists)
	{
		setInt(gShadowMapExists, _exists);
	}
	void SetShadowColor(vec3 _shadowColor)
	{
		setVec3(gShadowColor, _shadowColor);
	}

public:
	static const int32 C_ColorsStart = 0;
	static const int32 C_SpecularStart = 4;
	static const int32 C_Blend = 8;

private:
	int32 gColorMap[4];
	int32 gSpecularMap[4];

	int32 gBlend;

	int32 gShadowMapExists;
	int32 gShadowColor;
};