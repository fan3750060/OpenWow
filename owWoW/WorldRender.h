#pragma once

#include "WorldController.h"

class WorldRender : Renderable3DObject
{
public:
	WorldRender(WorldController* _WorldContoller);
	~WorldRender();
	
	inline Camera* GetTestCamera() { return m_TestCamera; }
	inline R_RenderBuffer* GetTestRB() { return m_TestRenderBuffer; }

	void PreRender3D();
	void Render3D();
	void PostRender3D();

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
};