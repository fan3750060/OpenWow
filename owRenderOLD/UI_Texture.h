#pragma once

#include "UIBase.h"

class CUI_Texture : public UIBase
{
public:
	CUI_Texture(RenderDevice* _RenderDevice) : 
		UIBase(_RenderDevice, "shaders/UI/UI_Texture")
	{
		gTexture = getLocation("gTexture");
	}

	//

	inline void SetTexture(int texture)
	{
		setTexture(gTexture, texture);
	}

private:
	int32 gTexture;
};