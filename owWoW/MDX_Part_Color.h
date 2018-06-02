#pragma once

#include "MDX_Headers.h"

class MDX_Part_Color
{
	friend class MDX;
	friend class MDX_Skin_Batch;
public:
	void init(IFile* f, M2Color& mcd, uint32* global);
	void calc(uint32 anim, uint32 time);

	vec4 getValue() { return vec4(vColor, vAlpha); }
	vec3 getValueColor() const { return vColor; }
	float getValueAlpha() { return vAlpha; }

private:
	Animated<vec3> color;
	Animated<float, short, ShortToFloat> alpha;

	vec3 vColor;
	float vAlpha;
};