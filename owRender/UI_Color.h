#pragma once

#include "UIBase.h"

class CUI_Color : public UIBase
{
public:
	CUI_Color(RenderDevice* _RenderDevice) : 
		UIBase(_RenderDevice, "shaders/UI/UI_Color")
    {
        gColor = getLocation("gColor");
    }

	inline void SetColor(Color color)
	{
		setVec4(gColor, vec4(color.red, color.green, color.blue, color.alpha));
	}

private:
    int32 gColor;
};