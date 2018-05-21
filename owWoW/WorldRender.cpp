#include "stdafx.h"

// General
#include "WorldRender.h"

WorldRender::WorldRender(WorldController * _WorldContoller)
	: m_WorldContoller(_WorldContoller)
{
	m_TestRenderBuffer = _Render->r.createRenderBuffer(_Config.windowSizeX, _Config.windowSizeY, R_TextureFormats::RGBA16F, true, 4, 0);

	m_TestCamera = new Camera;
	m_TestCamera->setupViewParams(45.0f, _Config.aspectRatio, 2.0f, 15000.0f);

	_Bindings->RegisterRenderable3DObject(this);
}

WorldRender::~WorldRender()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

void WorldRender::PreRender3D(double t, double dt)
{
	ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();

	_World->MDXM()->resetAnim();

	_PipelineGlobal->SetCamera(_Render->mainCamera);
	_PipelineGlobal->SetCameraFrustum(_Render->mainCamera);

	_Perfomance->FrameBegin();
}

void WorldRender::Render3D()
{
	_World->EnvM()->BeforeDraw();
	_Render->mainCamera->onPostUpdate();

	// Main frame

	//--------------------------------------------------
	// Geometry pass
	//--------------------------------------------------
	_Render->rb->setRenderBuffer();
	_Render->r.clear();
	RenderGeom();
}

void WorldRender::PostRender3D()
{
	//--------------------------------------------------
	// Postprocess pass
	//--------------------------------------------------
	_Render->rb->resetRenderBuffer(); // HACK!!!! MOVE RESET TO RenderDevice
	for (uint32 i = 0; i < 4; i++)
	{
		_Render->r.setTexture(i, _Render->rb->getRenderBufferTex(i), 0, 0);
	}
	_Render->r.clear(CLR_COLOR_RT0 | CLR_DEPTH);
	RenderPostprocess();

	// Result pass
	/*_Render->rb->resetRenderBuffer(); // HACK!!!! MOVE RESET TO RenderDevice
	_Render->r.setTexture(0, _Render->rbFinal->getRenderBufferTex(0), 0, R_TextureUsage::Texture);
	_Render->r.clear(CLR_COLOR_RT0 | CLR_DEPTH);
	DSResultQuad();*/

	//
	// SECONDS PASS
	//

	/*ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();

	// Conf test camera
	m_TestCamera->Position = _Render->mainCamera->Position + vec3(0, 1, 0) * 1000.0f;
	m_TestCamera->Roll = _Render->mainCamera->Roll;
	m_TestCamera->Pitch = -90.0f;
	m_TestCamera->Update();

	// Test frame
	_PipelineGlobal->SetCamera(m_TestCamera);

	// Geometry pass
	m_TestRenderBuffer->setRenderBuffer();
	_Render->r.clear();
	RenderGeom();
	//_PipelineGlobal->RenderCamera(_CameraFrustum);

	m_TestRenderBuffer->resetRenderBuffer();*/
}

//***************************************

void WorldRender::RenderGeom()
{
	//------------------------------------------------------------------------------
	// Draw sky from WMO
	//------------------------------------------------------------------------------
	_Render->r.setDepthTest(false);
	//if (_Config.draw_map_mdx)
	//{
	m_WorldContoller->Map()->RenderSky();
	//}

	//------------------------------------------------------------------------------
	// Draw sky from GLOBAL WMO
	//------------------------------------------------------------------------------
	_Render->r.setDepthTest(true);
	if (m_WorldContoller->Map()->m_WDT.MapHasGlobalWMO() && !_World->EnvM()->m_HasSky)
	{
		m_WorldContoller->Map()->SetOutOfBounds(false);
		m_WorldContoller->Map()->m_WDT.GetGlobalWMOInstance()->GetWMO()->drawSkybox();
	}

	//
	
	//

	//=== DEBUG
	//_EnvironmentManager->skies->DEBUG_Render();
	//=== DEBUG

	//=== DEBUG
	_World->EnvM()->dayNightPhase.Render_DEBUG(_Camera->Position);
	//=== DEBUG

	//

	//------------------------------------------------------------------------------
	// Map chunks DEBUG
	//------------------------------------------------------------------------------
	/*if (_Config.draw_map_chunk)
	{
		_Render->TechniquesMgr()->m_Debug_Normals->Bind();
		_Pipeline->Clear();
		_Render->TechniquesMgr()->m_Debug_Normals->SetPVW();

		m_WorldContoller->Map()->Render_DEBUG();

		_Render->TechniquesMgr()->m_Debug_Normals->Unbind();
	}*/

	//

	//------------------------------------------------------------------------------
	// Global WMO
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_MODELS_WMO_GLOBAL);
	if (m_WorldContoller->Map()->m_WDT.MapHasGlobalWMO())
	{
		m_WorldContoller->Map()->SetOutOfBounds(false);
		m_WorldContoller->Map()->m_WDT.GetGlobalWMOInstance()->Render();
	}
	PERF_STOP(PERF_MAP_MODELS_WMO_GLOBAL);

	//

	//------------------------------------------------------------------------------
	// WMOs
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_MODELS_WMOs);
	if (_Config.draw_map_wmo)
	{
		m_WorldContoller->Map()->RenderObjects();
	}
	PERF_STOP(PERF_MAP_MODELS_WMOs);

	//

	//------------------------------------------------------------------------------
	// Map MDXs
	//------------------------------------------------------------------------------
	PERF_START(PERF_MAP_MODELS_MDXs);
	if (_Config.draw_map_mdx)
	{
		m_WorldContoller->Map()->RenderModels();
	}
	PERF_STOP(PERF_MAP_MODELS_MDXs);
}

void WorldRender::RenderPostprocess()
{
	DSSimpleRenderPass();

	/*DirectionalLight light;
	light.Direction = vec3(_World->EnvM()->dayNightPhase.dayDir);
	light.ambient = _World->EnvM()->skies->GetColor(LightColors::LIGHT_COLOR_GLOBAL_AMBIENT);
	light.diffuse = _World->EnvM()->skies->GetColor(LightColors::LIGHT_COLOR_GLOBAL_DIFFUSE);
	light.specular = vec3(1.0f, 1.0f, 1.0f);
	DSDirectionalLightPass(light);*/

	if (_Config.drawfog)
	{
		DSFogRenderPass();
	}
}

void WorldRender::DSDirectionalLightPass(DirectionalLight& _light)
{
	_Render->TechniquesMgr()->m_POST_DirectionalLight->Bind();
	_Render->TechniquesMgr()->m_POST_DirectionalLight->SetCameraPos(_Camera->Position);

	_Render->TechniquesMgr()->m_POST_DirectionalLight->SetDirectionalLight(_light);

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->m_POST_DirectionalLight->Unbind();
}

void WorldRender::DSSimpleRenderPass()
{
	_Render->TechniquesMgr()->m_POST_Simple->Bind();
	_Render->TechniquesMgr()->m_POST_Simple->SetCameraPos(_Camera->Position);

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->m_POST_Simple->Unbind();
}

void WorldRender::DSFogRenderPass()
{
	_Render->TechniquesMgr()->m_POST_Fog->Bind();
	_Render->TechniquesMgr()->m_POST_Fog->SetCameraPos(_Camera->Position);

	_Render->TechniquesMgr()->m_POST_Fog->SetFogDistance(m_WorldContoller->EnvM()->skies->GetFog(LIGHT_FOG_DISTANCE));
	_Render->TechniquesMgr()->m_POST_Fog->SetFogModifier(m_WorldContoller->EnvM()->skies->GetFog(LIGHT_FOG_MULTIPLIER));
	_Render->TechniquesMgr()->m_POST_Fog->SetFogColor(m_WorldContoller->EnvM()->skies->GetColor(LIGHT_COLOR_FOG));

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->m_POST_Fog->Unbind();
}

void WorldRender::DSResultQuad()
{
	_Render->TechniquesMgr()->m_UI_Texture->Bind();

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuadVT();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->TechniquesMgr()->m_UI_Texture->Unbind();
}