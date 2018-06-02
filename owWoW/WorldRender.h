#pragma once

#include "WorldController.h"

class WorldRender : public CRenderable3DObject
{
public:
	WorldRender(WorldController* _WorldContoller);
	~WorldRender();
	
	inline Camera* GetTestCamera() { return m_TestCamera; }
	inline R_RenderBuffer* GetTestRB() { return m_TestRenderBuffer; }

	// IRenderable3D
	void PreRender3D(double t, double dt) override;
	void Render3D() override;
	void PostRender3D() override;

	void RenderGeom();
	void RenderPostprocess();
	void DSDirectionalLightPass(DirectionalLight& _light);
	void DSSimpleRenderPass();
	void DSFogRenderPass();
	void DSResultQuad();

private:
	Camera*          m_TestCamera;
	R_RenderBuffer*  m_TestRenderBuffer;

private:
	WorldController* m_WorldContoller;
	CGroupVideo&    groupVideo;
	CGroupQuality&    groupQuality;
};