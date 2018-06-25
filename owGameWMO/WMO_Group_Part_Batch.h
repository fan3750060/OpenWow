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

	uint32 getBlendMode() const { return m_WMOMaterial->getBlendMode(); }

	void Render();

private:
	const WMO*					m_ParentWMO;
	const WMO_Group_BatchDef	m_Proto;
	const WMO_Part_Material*	m_WMOMaterial;
	BoundingBox					m_Bounds;

	const CGroupQuality&		m_Quality;
};

struct WMO_Group_Part_BatchCompare
{
	bool operator() (const WMO_Group_Part_Batch* left, const WMO_Group_Part_Batch* right) const
	{
		return left->getBlendMode() < right->getBlendMode();
	}
};