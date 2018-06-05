#include "stdafx.h"

// General
#include "Pipeline.h"

void Pipeline::SetCamera(Camera* _camera)
{
	assert1(_camera != nullptr);

	camera = _camera;
}

void Pipeline::SetCameraFrustum(Camera* _camera)
{
	assert1(_camera != nullptr);

	cameraFrustum = _camera;
}