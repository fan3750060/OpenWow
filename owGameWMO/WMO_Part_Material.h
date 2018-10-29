#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Part_Material : public Material
{
public:
	WMO_Part_Material(const std::weak_ptr<const WMO> _parentWMO, const SWMO_MaterialDef& _proto);

	void set() const;
	
	uint32 getBlendMode() const { return m_Proto.blendMode; }

private:
	const SWMO_MaterialDef         m_Proto;

	// Parent
	const std::weak_ptr<const WMO> m_ParentWMO;
	const CGroupQuality&           m_QualitySettings;
};