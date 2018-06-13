#pragma once

#include "GeometryBase.h"

class CDebug_GeometryPass : public GeometryBase
{
public:
	CDebug_GeometryPass(RenderDevice* _RenderDevice) : 
		GeometryBase(_RenderDevice, "shaders/Debug/Debug")
    {
        gColor = getLocation("gColor");
    }

	//

	inline void SetColor4(cvec4 _color)
	{
		setVec4(gColor, _color);
	}

private:
    int32 gColor;
};