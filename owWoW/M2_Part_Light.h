#pragma once

#include "M2_Types.h"

enum ModelLightTypes
{
	MODELLIGHT_DIRECTIONAL = 0,
	MODELLIGHT_POINT = 1
};

class CM2_Part_Light
{
	friend class MDX;
public:
	void init(IFile* f, SM2_Light& mld, const vector<SM2_Loop>* global);
	void setup(uint32 time, uint32 globalTime);

private:
	int type, parent;
	vec3 pos, tpos, dir, tdir;
	M2_Animated<vec3> diffColor, ambColor;
	M2_Animated<float> diffIntensity, ambIntensity;
};