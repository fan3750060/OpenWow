#include "stdafx.h"

// General
#include "MDX_Part_Camera.h"

// Additional
#include "WorldController.h"

MDX_Part_Camera::MDX_Part_Camera() :
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
}

void MDX_Part_Camera::init(IFile* f, M2Camera& mcd, uint32* global)
{
	nearclip = mcd.near_clip;
	farclip = mcd.far_clip;

	pos = mcd.position_base.toXZmY();
	target = mcd.target_position_base.toXZmY();

	tPos.init(mcd.positions, f, global);
	tPos.fix(Fix_XZmY);

	tTarget.init(mcd.target_position, f, global);
	tTarget.fix(Fix_XZmY);

	tRoll.init(mcd.roll, f, global);
	fov = mcd.fov;


	camera.m_UseDir = true;
}

void MDX_Part_Camera::setup(int time)
{
	vec3 p = pos + tPos.getValue(0, time, _World->EnvM()->globalTime);
	vec3 t = target + tTarget.getValue(0, time, _World->EnvM()->globalTime);
	vec3 u(0, 1, 0);

	roll = tRoll.getValue(0, time, _World->EnvM()->globalTime) / Math::Pi * 180.0f;

	camera.setupViewParams(fov, m_VideoSettings.aspectRatio, nearclip, farclip);

	camera.Position = p;
	camera.Direction = t - p;
	camera.CameraUp = u;

	camera.SetNeedUpdate();
	camera.Update(0,0);
	//camera.CreateRenderable();
}