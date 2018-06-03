#pragma once

#include "M2_Types.h"

class CM2_Part_Camera
{
	friend class MDX;
public:
	CM2_Part_Camera();

	void init(IFile* f, SM2_Camera& mcd, const vector<SM2_Loop>* global);
	void setup(uint32 time, uint32 globalTime);

	Camera* GetCamera() { return &camera; }

private:
	float farclip;
	float nearclip;

	M2_Animated<vec3> tPos;
	vec3 pos;

	M2_Animated<vec3> tTarget;
	vec3 target;

	M2_Animated<float> tRoll;
	float roll;

	float fov;

	Camera camera;

	CGroupVideo& m_VideoSettings;
};