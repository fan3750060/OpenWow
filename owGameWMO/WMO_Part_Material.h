#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Part_Material
{
public:
	WMO_Part_Material(const WMO* _parentWMO, const SWMO_MaterialDef& _proto);

#ifdef GAME_WMO_INCLUDE_WM2
	void fillRenderState(RenderState* _state) const;
#endif
	void set() const;
	
	uint32 getBlendMode() const { return m_Proto.blendMode; }

private:
	const WMO*					m_ParentWMO;
	const SWMO_MaterialDef		m_Proto;

	std::shared_ptr<Texture>			m_DiffuseTexture[3];

	CGroupQuality&				m_QualitySettings;
};