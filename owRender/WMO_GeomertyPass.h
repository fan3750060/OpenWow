#pragma once

#include "technique.h"

class WMO_GeomertyPass : public Technique
{
public:
    WMO_GeomertyPass(RenderDevice* _RenderDevice) : Technique(_RenderDevice, "shaders/WMO")
    {
        gColorMap = getLocation("gColorMap");
        gSpecularMap = getLocation("gSpecularMap");
        gHasMOCV = getLocation("gHasMOCV");
        gDiffuseColor = getLocation("gDiffuseColor");
    }

    inline void SetColorTextureUnit(int TextureUnit)
    {
        setTexture(gColorMap, TextureUnit);
    }

    inline void SetSpecularTextureUnit(int TextureUnit)
    {
        setTexture(gSpecularMap, TextureUnit);
    }

    //

    void SetHasMOCV(bool hasMOCV)
    {
        setInt(gHasMOCV, hasMOCV);
    }

    void SetDiffuseColor(vec3 _Color)
    {
        setVec3(gDiffuseColor, _Color);
    }

private:
    int32 gColorMap;
    int32 gSpecularMap;
    int32 gHasMOCV;
    int32 gDiffuseColor;
};