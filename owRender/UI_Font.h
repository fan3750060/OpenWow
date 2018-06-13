#pragma once

#include "UIBase.h"

class CUI_Font : public UIBase
{
public:
	CUI_Font(RenderDevice* _RenderDevice) : 
		UIBase(_RenderDevice, "shaders/UI/UI_Font")
    {
        gFontTexture = getLocation("gFontTexture");
        gFontColor = getLocation("gFontColor");
        gOffset = getLocation("gOffset");
    }

	//

	inline void SetFontTexture(int texture)
	{
		setTexture(gFontTexture, texture);
	}

	inline void SetFontColor(cvec3 _color)
	{
		setVec3(gFontColor, _color);
	}

	inline void SetCharOffset(cvec2 _color)
	{
		setVec2(gOffset, _color);
	}

private:
    int32 gFontTexture;
    int32 gFontColor;
    int32 gOffset;
};