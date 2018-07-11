#include "stdafx.h"

// General
#include "M2_Part_Material.h"

// M2Blend converter
struct
{
	SM2_Material::BlendModes	M2Blend;
	uint8						EGxBLend;
} M2Blend_To_EGxBlend[SM2_Material::COUNT] =
{
	{ SM2_Material::M2BLEND_OPAQUE,			0 },
	{ SM2_Material::M2BLEND_ALPHA_KEY,		1 },
	{ SM2_Material::M2BLEND_ALPHA,			2 },
	{ SM2_Material::M2BLEND_NO_ALPHA_ADD,	10 },
	{ SM2_Material::M2BLEND_ADD,			3 },
	{ SM2_Material::M2BLEND_MOD,			4 },
	{ SM2_Material::M2BLEND_MOD2X,			5 }
};

CM2_Part_Material::CM2_Part_Material(const SM2_Material& _proto)
{
	m_IsLightingDisable = _proto.flags.UNLIT;
	m_IsFogDisable = _proto.flags.UNFOGGED;
	m_IsTwoSided = _proto.flags.TWOSIDED;
	m_DepthTestEnabled = _proto.flags.DEPTHTEST == 0;
	m_DepthMaskEnabled = _proto.flags.DEPTHWRITE == 0;

	m_M2BlendMode = _proto.m_BlendMode;
}

void CM2_Part_Material::fillRenderState(RenderState* _state) const
{
	_state->setCullMode(m_IsTwoSided ? R_CullMode::RS_CULL_NONE : R_CullMode::RS_CULL_BACK);
	_state->setDepthTest(m_DepthTestEnabled);
	_state->setDepthMask(m_DepthMaskEnabled);
	_Render->getRenderStorage()->SetEGxBlend(_state, M2Blend_To_EGxBlend[m_M2BlendMode].EGxBLend);
}

void CM2_Part_Material::Set() const
{
	_Render->r.setCullMode(m_IsTwoSided ? R_CullMode::RS_CULL_NONE : R_CullMode::RS_CULL_BACK);
	_Render->r.setDepthTest(m_DepthTestEnabled);
	_Render->r.setDepthMask(m_DepthMaskEnabled);
	_Render->getRenderStorage()->SetEGxBlend(_Render->r.getState(), M2Blend_To_EGxBlend[m_M2BlendMode].EGxBLend);
}
