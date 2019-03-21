#include "stdafx.h"

// General
#include "Material.h"

// Additional
#include "Render.h"
#include "RenderDevice.h"
#include "RenderStorage.h"

Material::Material() :
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	m_BlendEGxBlendIndex = -1;
	m_BlendEnabled = false;
	m_Blend_SrcFunc = BS_BLEND_SRC_ALPHA;
	m_Blend_DstFunc = BS_BLEND_INV_SRC_ALPHA;

	m_IsTwoSided = false;

	m_DepthTest = true;
	m_DepthWrite = true;

	m_DiffuseTexture = 0;
	m_SpecularTexture = 0;
}

void Material::Set() const
{
	//_Render->r.setAlphaToCoverage(true);
	if (m_BlendEGxBlendIndex != -1)
	{
		_Render->getRenderStorage()->SetEGxBlend(_Render->r.getState(), m_BlendEGxBlendIndex);
	}
	else
	{
		_Render->r.setBlendMode(m_BlendEnabled, m_Blend_SrcFunc, m_Blend_DstFunc);
	}
	_Render->r.setCullMode(m_IsTwoSided ? R_CullMode::RS_CULL_NONE : R_CullMode::RS_CULL_BACK);
	_Render->r.setDepthTest(m_DepthTest);
	_Render->r.setDepthMask(m_DepthWrite);

	_Render->r.setTexture(C_DiffuseTextureIndex, m_DiffuseTexture, m_QualitySettings.Texture_Sampler | SS_ADDR_WRAP, 0);
	
	if (m_SpecularTexture != nullptr)
	{
		_Render->r.setTexture(C_SpecularTextureIndex, m_SpecularTexture, m_QualitySettings.Texture_Sampler | SS_ADDR_WRAP, 0);
	}

}

void Material::Restore()
{
	_Render->r.setBlendMode(true, R_BlendFunc::BS_BLEND_SRC_ALPHA, R_BlendFunc::BS_BLEND_INV_SRC_ALPHA);
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
	_Render->r.setDepthTest(true);
	_Render->r.setDepthMask(true);
}
