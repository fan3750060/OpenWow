#pragma once

#include "M2_Types.h"

class CM2_Part_Material
{
public:
	CM2_Part_Material(const SM2_Material& _proto);

	void fillRenderState(RenderState* _state) const;
	void Set() const;

	SM2_Material::BlendModes getBlendMode() const { return m_M2BlendMode; }


private:
	bool m_IsLightingDisable;
	bool m_IsFogDisable;
	bool m_IsTwoSided;
	bool m_DepthTestEnabled;
	bool m_DepthMaskEnabled;

	SM2_Material::BlendModes m_M2BlendMode;
};