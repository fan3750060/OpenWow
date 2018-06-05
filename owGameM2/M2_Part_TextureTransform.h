#pragma once

#include "M2_Types.h"

class CM2_Part_TextureTransform
{
public:
	CM2_Part_TextureTransform(IFile* f, const SM2_TextureTransform& _proto, cGlobalLoopSeq global);

	void calc(uint32 anim, uint32 time, uint32 globalTime);

	cmat4 getValue() const { return matrix; }

private:
	M2_Animated<vec3>		trans;
	M2_Animated<Quaternion> roll;
	M2_Animated<vec3>		scale;

	vec3					transValue;
	Quaternion				rollValue;
	vec3					scaleVal;

	mat4					matrix;
};