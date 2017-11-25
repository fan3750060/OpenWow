#pragma once

#include "technique.h"

class UI_Color : public Technique
{
public:
	UI_Color() : Technique("shaders/UI_Color") 
    {
        gColor = getLocation("gColor");
    }

	//

	inline void SetColor(Color color)
	{
		setVec4(gColor, vec4(color.red, color.green, color.blue, color.alpha));
	}

private:
    int32 gColor;
};