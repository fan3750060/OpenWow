#pragma once

#include "PostprocessBase.h"

class POST_Fog : public PostprocessBase
{
public:
    POST_Fog(RenderDevice* _RenderDevice) : 
		PostprocessBase(_RenderDevice, "shaders/Postprocess/Postprocess_Fog.fs") 
	{
		gFogDistance = getLocation("gFogDistance");
		gFogModifier = getLocation("gFogModifier");
		gFogColor = getLocation("gFogColor");
	}

	//--

    void SetFogDistance(float _distance)
    {
        setFloat(gFogDistance, _distance);
    }

    void SetFogModifier(float _modifier)
    {
        setFloat(gFogModifier, _modifier);
    }

    void SetFogColor(cvec3 _color)
    {
        setVec3(gFogColor, _color);
    }

private:
	int32 gFogDistance;
	int32 gFogModifier;
	int32 gFogColor;
};