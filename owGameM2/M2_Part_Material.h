#pragma once

#include "M2_Types.h"

class CM2_Part_Material
{
public:
	CM2_Part_Material(const SM2_Material& _proto);

	void Set() const;

	SM2_Material::BlendModes getBlendMode() const { return m_M2BlendMode; }

	DepthStencilState::DepthMode GetDepthMode() const;
	BlendState::BlendMode GetBlendMode() const;
	RasterizerState::CullMode GetCullMode() const;

private:
	bool m_IsLightingDisable;
	bool m_IsFogDisable;
	RasterizerState::CullMode m_CullMode;

	bool m_DepthTest;
	DepthStencilState::DepthWrite m_DepthWrite;

	SM2_Material::BlendModes m_M2BlendMode;
};