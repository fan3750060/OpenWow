#include "stdafx.h"

// General
#include "M2_Part_Camera.h"

CM2_Part_Camera::CM2_Part_Camera(IFile* f, const SM2_Camera& _proto, cGlobalLoopSeq global) :
	m_VideoSettings(GetSettingsGroup<CGroupVideo>())
{
	nearclip = _proto.near_clip;
	farclip = _proto.far_clip;

	pos = _proto.position_base.toXZmY();
	target = _proto.target_position_base.toXZmY();

	tPos.init(_proto.positions, f, global, Fix_XZmY);
	tTarget.init(_proto.target_position, f, global, Fix_XZmY);

	tRoll.init(_proto.roll, f, global);
	fov = _proto.fov;


	camera.m_UseDir = true;
}

void CM2_Part_Camera::setup(uint32 time, uint32 globalTime)
{
	vec3 p;
	vec3 t;
	if (tPos.uses())
	{
		p = pos + tPos.getValue(0, time, globalTime);
	}
	if (tTarget.uses())
	{
		t = target + tTarget.getValue(0, time, globalTime);
	}
	vec3 u(0, 1, 0);

	if (tRoll.uses())
	{
		roll = tRoll.getValue(0, time, globalTime) / Math::Pi * 180.0f;
	}

	camera.setupViewParams(fov, m_VideoSettings.aspectRatio, nearclip, farclip);

	camera.Position = p;
	camera.Direction = t - p;
	camera.CameraUp = u;

	camera.SetNeedUpdate();
	camera.Update(0,0);
	//camera.CreateRenderable();
}