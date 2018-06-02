#include "stdafx.h"

// General
#include "Pipeline.h"
#include "TechniquesManager.h"

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

/*void Pipeline::SetCamera(const vec3& _position, const vec3& _rotation, const vec3& _up)
{
	vec3 CameraRight = glm::normalize(glm::cross(_rotation, _up));
	vec3 CameraUp = glm::normalize(glm::cross(CameraRight, _rotation));

	view = glm::lookAt(_position, _position + _rotation, CameraUp);
	camera = nullptr;
}*/
