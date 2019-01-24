#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Part_Material : public MaterialWrapper
{
public:
	WMO_Part_Material(const std::weak_ptr<const WMO> _parentWMO, const SWMO_MaterialDef& _proto);
	virtual ~WMO_Part_Material();
	
	uint32 getBlendMode() const { return m_Proto.blendMode; }

private:
	const SWMO_MaterialDef         m_Proto;

	__declspec(align(16)) struct MaterialProperties
	{
		uint32 m_BlendMode;
		vec3   m_Pad;
		//-------------------------- ( 32 bytes )
	};
	MaterialProperties*            m_pProperties;

	// Parent
	const std::weak_ptr<const WMO> m_ParentWMO;
	const CGroupQuality&           m_QualitySettings;
};