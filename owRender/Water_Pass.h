#pragma once

#include "technique.h"

class Water_Pass : public Technique
{
public:
    Water_Pass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/Water")
    {
        gColorMap = getLocation("gColorMap");
        gSpecularMap = getLocation("gSpecularMap");

        gColorLight = getLocation("gColorLight");
        gColorDark = getLocation("gColorDark");
        gShallowAlpha = getLocation("gShallowAlpha");
        gDeepAlpha = getLocation("gDeepAlpha");
    }

    inline void SetColorTextureUnit(int TextureUnit)
    {
        setTexture(gColorMap, TextureUnit);
    }

    inline void SetSpecularTextureUnit(int TextureUnit)
    {
        setTexture(gSpecularMap, TextureUnit);
    }

    void SetWaterColorLight(cvec3 _Color)
    {
        setVec3(gColorLight, _Color);
    }

    void SetWaterColorDark(cvec3 _Color)
    {
        setVec3(gColorDark, _Color);
    }

    void SetShallowAlpha(float _value)
    {
        setFloat(gShallowAlpha, _value);
    }

    void SetDeepAlpha(float _value)
    {
        setFloat(gDeepAlpha, _value);
    }

private:
    int8 gColorMap;
    int8 gSpecularMap;
    int8 gColorLight;
    int8 gColorDark;
    int8 gShallowAlpha;
    int8 gDeepAlpha;
};