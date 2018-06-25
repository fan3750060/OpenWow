#pragma once

#include "GeometryBase.h"

class CWMO_GeomertyPass : public GeometryBase
{
public:
    CWMO_GeomertyPass(RenderDevice* _RenderDevice) : GeometryBase(_RenderDevice, "shaders/WMO")
    {
        gColorMap[0] = getLocation("gColorMap0");
		gColorMap[1] = getLocation("gColorMap1");
		gColorMap[2] = getLocation("gColorMap2");
        gSpecularMap = getLocation("gSpecularMap");

		gBlendMode = getLocation("gBlendMode");

        gHasMOCV = getLocation("gHasMOCV");

		gUseAmbColor = getLocation("gUseAmbColor");
        gAmbColor = getLocation("gAmbColor");
    }

    inline void SetColorTextureUnit(int index, int TextureUnit)
    {
        setTexture(gColorMap[index], TextureUnit);
    }

    inline void SetSpecularTextureUnit(int TextureUnit)
    {
        setTexture(gSpecularMap, TextureUnit);
    }

    //

	void SetBlendMode(int _blendMode)
	{
		setInt(gBlendMode, _blendMode);
	}

    void SetHasMOCV(bool hasMOCV)
    {
        setInt(gHasMOCV, hasMOCV);
    }

	void SetUseAmbColor(bool _use)
	{
		setInt(gUseAmbColor, _use);
	}
    void SetAmbColor(vec4 _Color)
    {
        setVec4(gAmbColor, _Color);
    }

private:
    int32 gColorMap[3];
    int32 gSpecularMap;

	int32 gBlendMode;

    int32 gHasMOCV;
	int32 gUseAmbColor;
    int32 gAmbColor;
};