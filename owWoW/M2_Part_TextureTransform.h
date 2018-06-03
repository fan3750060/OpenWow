#pragma once

#include "M2_Types.h"

class CM2_Part_TextureTransform
{
	friend class MDX;
	friend class MDX_Skin_Batch;
public:
	void init(IFile* f, SM2_TextureTransform& mta, const vector<SM2_Loop>* global);
	void calc(uint32 anim, uint32 time, uint32 globalTime);

	cmat4 getValue() const { return matrix; }

private:
	M2_Animated<vec3> trans;
	M2_Animated<Quaternion> roll;
	M2_Animated<vec3> scale;

	vec3 tval;
	Quaternion rval;
	vec3 sval;

	mat4 matrix;
};