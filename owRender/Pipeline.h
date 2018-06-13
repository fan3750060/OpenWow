#pragma once

class Pipeline
{
	CLASS_INSTANCE(Pipeline);
public:
	Pipeline() : 
		camera(nullptr) 
	{}

	void SetCamera(Camera* _camera);
	void SetCameraFrustum(Camera* _camera);

	inline const mat4& GetProjection() { return camera->getProjMat(); }
	inline const mat4& GetView() { return camera->getViewMat(); }

	inline Camera* GetCamera() { return camera; }
	inline Camera* GetCameraFrustum() { return cameraFrustum; }

private:
	Camera* camera;
	Camera* cameraFrustum;

private:
	float nh;
	float nw;

	float fh;
	float fw;

	float nearDist;
	float farDist;
};

#define _PipelineGlobal Pipeline::instance()

#define _Camera  Pipeline::instance()->GetCamera()
#define _CameraFrustum   Pipeline::instance()->GetCameraFrustum()