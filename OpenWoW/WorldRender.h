#pragma once

#include "WorldController.h"

class WorldRender : public CRenderable3DObject
{
public:
	WorldRender(WorldController* _WorldContoller);
	~WorldRender();
	
	// IRenderable3D
	bool PreRender3D() override;
	void Render3D() override;
	void PostRender3D() override;

	void RenderPostprocess();

	void DSDirectionalLightPass(DirectionalLight& _light);
	void DSFogRenderPass();
	void DSResultQuad();

private:
	WorldController*	m_WorldContoller;
	CGroupVideo&		m_VideoSettings;
	CGroupQuality&		groupQuality;
};