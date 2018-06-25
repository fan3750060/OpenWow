#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Part_Material : public CRefItem
{
public:
	WMO_Part_Material(const WMO* _parentWMO, const WMO_MaterialDef& _proto);

	void set() const;

	uint32 getBlendMode() const { return m_Proto.blendMode; }


private:
	const WMO*				m_ParentWMO;
	const WMO_MaterialDef	m_Proto;


	SmartTexturePtr			m_DiffuseTexture[3];

	CGroupQuality& m_QualitySettings;
};