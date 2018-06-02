#pragma once

#include "MDX_Headers.h"

class MDX_Part_Camera
{
	friend class MDX;
public:
	MDX_Part_Camera();

	void init(IFile* f, M2Camera& mcd, uint32* global);
	void setup(int time = 0);

	Camera* GetCamera() { return &camera; }

private:
	float farclip;
	float nearclip;

	Animated<vec3> tPos;
	vec3 pos;

	Animated<vec3> tTarget;
	vec3 target;

	Animated<float> tRoll;
	float roll;

	float fov;

	Camera camera;

	CGroupVideo& m_VideoSettings;
};