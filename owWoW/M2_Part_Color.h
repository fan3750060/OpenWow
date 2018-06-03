#pragma once

#include "M2_Types.h"

class CM2_Part_Color
{
	friend class MDX;
	friend class MDX_Skin_Batch;
public:
	CM2_Part_Color();

	void init(IFile* f, SM2_Color& mcd, const vector<SM2_Loop>* global);
	void calc(uint32 anim, uint32 time, uint32 globalTime);

	vec4 getValue() { return vec4(vColor, vAlpha); }
	cvec3 getValueColor() const { return vColor; }
	float getValueAlpha() { return vAlpha; }

private:
	M2_Animated<vec3> color;
	M2_Animated<float, short, ShortToFloat> alpha;

	vec3 vColor;
	float vAlpha;
};