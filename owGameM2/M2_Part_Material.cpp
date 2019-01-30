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
	m_CullMode = (_proto.flags.TWOSIDED != 0) ? RasterizerState::CullMode::None : RasterizerState::CullMode::Back;
	m_DepthTest = (_proto.flags.DEPTHTEST == 0);
	m_DepthWrite = (_proto.flags.DEPTHWRITE == 0) ? DepthStencilState::DepthWrite::Enable : DepthStencilState::DepthWrite::Disable;

	m_M2BlendMode = _proto.m_BlendMode;
}

void CM2_Part_Material::Set() const
{
	/*_Render->r.setCullMode(m_IsTwoSided ? R_CullMode::RS_CULL_NONE : R_CullMode::RS_CULL_BACK);
	_Render->r.setDepthTest(m_DepthTestEnabled);
	_Render->r.setDepthMask(m_DepthMaskEnabled);
	_Render->getRenderStorage()->SetEGxBlend(_Render->r.getState(), M2Blend_To_EGxBlend[m_M2BlendMode].EGxBLend);*/
}

DepthStencilState::DepthMode CM2_Part_Material::GetDepthMode() const
{
	return DepthStencilState::DepthMode(m_DepthTest, m_DepthWrite);
}

/*BlendState::BlendMode CM2_Part_Material::GetBlendMode() const
{
	switch (m_M2BlendMode)
	{
	case 0: // Opaque
		return BlendState::BlendMode(false, false,
			BlendState::BlendFactor::One,
			BlendState::BlendFactor::Zero);
		break;

	case 1: // Combiners_Mod
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::One, BlendState::BlendFactor::Zero);
		break;

	case 2: // Combiners_Decal 
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
		break;

	case 3: // Combiners_Add 
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcColor, BlendState::BlendFactor::DstColor,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::DstAlpha);
		break;

	case 4: // Combiners_Mod2x 
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::One,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::One);
		break;

	case 5: // Combiners_Fade 
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
		break;


	default:
		fail1();
	}
}*/

BlendState::BlendMode CM2_Part_Material::GetBlendMode() const
{
	switch (M2Blend_To_EGxBlend[m_M2BlendMode].EGxBLend)
	{
	case 0: // Opaque
		return BlendState::BlendMode(false, false,
			BlendState::BlendFactor::One,
			BlendState::BlendFactor::Zero);
		break;

	case 1: // AlphaKey
		return BlendState::BlendMode(false, false,
			BlendState::BlendFactor::One, BlendState::BlendFactor::Zero);
		break;

	case 2: // Alpha
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::One, BlendState::BlendFactor::OneMinusSrcAlpha);
		break;

	case 3: // Add
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::One,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::Zero, BlendState::BlendFactor::One);
		break;

	case 4: // Mod
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::DstColor, BlendState::BlendFactor::Zero,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::DstAlpha, BlendState::BlendFactor::Zero);
		break;

	case 5: // Mod2x
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::DstColor, BlendState::BlendFactor::SrcColor,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::DstAlpha, BlendState::BlendFactor::SrcAlpha);
		break;

	case 6: // ModAdd
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::DstColor, BlendState::BlendFactor::One,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::DstAlpha, BlendState::BlendFactor::One);
		break;

	case 7: // InvSrcAlphaAdd
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendFactor::One,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendFactor::One);
		break;

	case 8: // InvSrcAlphaOpaque
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendFactor::Zero,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendFactor::Zero);
		break;

	case 9: // SrcAlphaOpaque
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::Zero,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::Zero);
		break;

	case 10: // NoAlphaAdd
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::One, BlendState::BlendFactor::One,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::Zero, BlendState::BlendFactor::One);

	case 11: // ConstantAlpha
		//GLSetBlend(true, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA, GL_CONSTANT_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
		fail2("Constant alpha EGxBlend doesn't support");
		break;

	case 12: // Screen
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::OneMinusDstColor, BlendState::BlendFactor::One,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::One, BlendState::BlendFactor::Zero);
		break;

	case 13: // BlendAdd
		return BlendState::BlendMode(true, false,
			BlendState::BlendFactor::One, BlendState::BlendFactor::OneMinusSrcAlpha,
			BlendState::BlendOperation::Add,
			BlendState::BlendFactor::One, BlendState::BlendFactor::OneMinusSrcAlpha);
		break;

	default:
		fail1();
	}
}

RasterizerState::CullMode CM2_Part_Material::GetCullMode() const
{
	return m_CullMode;
}
