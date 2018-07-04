#pragma once

#include "GeometryBase.h"

class CMCNK_Pass : public GeometryBase
{
public:
	CMCNK_Pass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/Map/MapChunk.vs", "shaders/Map/MapChunk.fs")
    {
        for (uint8 i = 0; i < 4; i++)
        {
            gColorMap[i] = getLocation((string("gColorMap[") + std::to_string(i) + "]").c_str());
            gSpecularMap[i] = getLocation((string("gSpecularMap[") + std::to_string(i) + "]").c_str());
        }

		gIsNortrend = getLocation("gIsNortrend");
		gIsMCCVExists = getLocation("gIsMCCVExists");
        gLayersCount = getLocation("gLayersCount");
        gBlend = getLocation("gBlend");
		gIsLowRes = getLocation("gIsLowRes");

        gShadowMapExists = getLocation("gShadowMapExists");
        gShadowColor = getLocation("gShadowColor");
    }

	//--

	void SetColorTextureUnit(int _number, int TextureUnit)
	{
		setTexture(gColorMap[_number], CMCNK_Pass::C_ColorsStart + TextureUnit);
	}
    void SetSpecularTextureUnit(int _number, int TextureUnit)
    {
        setTexture(gSpecularMap[_number], CMCNK_Pass::C_SpecularStart + TextureUnit);
    }
	void SetShadowMapExists(bool _exists)
	{
		setInt(gShadowMapExists, _exists);
	}
	void SetShadowColor(vec3 _shadowColor)
	{
		setVec3(gShadowColor, _shadowColor);
	}

	//--

	void SetIsNortrend(bool _exists)
	{
		setInt(gIsNortrend, _exists);
	}
	void SetIsMCCVExists(bool _exists)
	{
		setInt(gIsMCCVExists, _exists);
	}
	void SetLayersCount(int _layersCount)
	{
		setInt(gLayersCount, _layersCount);
	}
	void SetBlendBuffer()
	{
		setTexture(gBlend, CMCNK_Pass::C_Blend);
	}
	void SetIsLowRes(bool _exists)
	{
		setInt(gIsLowRes, _exists);
	}

public:
	static const int32 C_ColorsStart = 0;
	static const int32 C_SpecularStart = 4;
	static const int32 C_Blend = 8;

private:
	int32 gColorMap[4];
	int32 gSpecularMap[4];

	int32 gShadowMapExists;
	int32 gShadowColor;

	int32 gIsNortrend;
	int32 gIsMCCVExists;
	int32 gLayersCount;
	int32 gBlend;
	int32 gIsLowRes;
};