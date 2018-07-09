#include "stdafx.h"

// General
#include "WorldRender.h"



WorldRender::WorldRender(WorldController * _WorldContoller)	: 
	m_WorldContoller(_WorldContoller),
	m_VideoSettings(GetSettingsGroup<CGroupVideo>()),
	groupQuality(GetSettingsGroup<CGroupQuality>())
{
	_Bindings->RegisterRenderable3DObject(this, 6);
}

WorldRender::~WorldRender()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

bool WorldRender::PreRender3D()
{
	ADT_WMO_Instance::reset();
	ADT_MDX_Instance::reset();

	_Render->getTechniquesMgr()->PreRender3D(_Render->getCamera(), _Render->m_RenderBuffer);

	return true;
}

void WorldRender::Render3D()
{
	_Render->BindRBs();
}

void WorldRender::PostRender3D()
{
	_Render->UnbindRBs();

	RenderPostprocess();
}

void WorldRender::RenderPostprocess()
{
	_Render->PostprocessSimple();

	/*DirectionalLight light;
	light.Direction = vec3(_World->EnvM()->dayNightPhase.dayDir);
	light.ambient = _World->EnvM()->m_SkyManager->GetColor(LightColors::LIGHT_COLOR_GLOBAL_AMBIENT);
	light.diffuse = _World->EnvM()->m_SkyManager->GetColor(LightColors::LIGHT_COLOR_GLOBAL_DIFFUSE);
	light.specular = vec3(1.0f, 1.0f, 1.0f);
	DSDirectionalLightPass(light);*/

	if (groupQuality.drawfog)
	{
		float fogDist = m_WorldContoller->EnvM()->m_SkyManager->GetFog(LightFogs::LIGHT_FOG_DISTANCE);
		groupQuality.UpdateByFog(fogDist);

		DSFogRenderPass();
	}
	else
	{
		//groupQuality.InitDefault();
	}
}

void WorldRender::DSDirectionalLightPass(DirectionalLight& _light)
{
	_Render->getTechniquesMgr()->Postprocess_Light_Direction->Bind();
	_Render->getTechniquesMgr()->Postprocess_Light_Direction->setCameraPos(_Render->getCamera()->Position);

	_Render->getTechniquesMgr()->Postprocess_Light_Direction->SetDirectionalLight(_light);

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->getTechniquesMgr()->Postprocess_Light_Direction->Unbind();
}

void WorldRender::DSFogRenderPass()
{
	_Render->getTechniquesMgr()->Postprocess_Fog->Bind();
	_Render->getTechniquesMgr()->Postprocess_Fog->setCameraPos(_Render->getCamera()->Position);

	_Render->getTechniquesMgr()->Postprocess_Fog->SetFogDistance(m_WorldContoller->EnvM()->m_SkyManager->GetFog(LightFogs::LIGHT_FOG_DISTANCE));
	_Render->getTechniquesMgr()->Postprocess_Fog->SetFogModifier(m_WorldContoller->EnvM()->m_SkyManager->GetFog(LightFogs::LIGHT_FOG_MULTIPLIER));
	_Render->getTechniquesMgr()->Postprocess_Fog->SetFogColor(m_WorldContoller->EnvM()->m_SkyManager->GetColor(LightColors::LIGHT_COLOR_FOG));

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuad();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->getTechniquesMgr()->Postprocess_Fog->Unbind();
}

void WorldRender::DSResultQuad()
{
	_Render->getTechniquesMgr()->UI_Texture->Bind();

	_Render->r.setDepthTest(false);
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);

	_Render->RenderQuadVT();

	_Render->r.setBlendMode(false);
	_Render->r.setDepthTest(true);

	_Render->getTechniquesMgr()->UI_Texture->Unbind();
}