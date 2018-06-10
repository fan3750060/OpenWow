#pragma once

#include "WMO_Headers.h"

#include "WMO_Part_Material.h"

// FORWARD BEGIN
class WMO;
// FORWARD END

class WMO_Group_Part_Batch : public CRefItem
{
public:
	WMO_Group_Part_Batch(const WMO* _parentWMO, const WMO_Group_BatchDef& _proto);

	void Render();

private:
	const WMO*					m_ParentWMO;
	const WMO_Group_BatchDef	m_Proto;
	const WMO_Part_Material*	m_WMOMaterial;
	Material					__material;

	const CGroupQuality& m_Quality;
};