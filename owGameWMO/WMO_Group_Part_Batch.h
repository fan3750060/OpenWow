#pragma once

#include "WMO_Headers.h"

#include "WMO_Part_Material.h"

// FORWARD BEGIN
class WMO;
class WMO_Group;
// FORWARD END

class WMO_Group_Part_Batch
{
public:
	WMO_Group_Part_Batch(const WMO* _parentWMO, const WMO_Group* _parentGroup, const SWMO_Group_BatchDef& _proto);

#ifdef GAME_WMO_INCLUDE_WM2
	void setGeom(std::shared_ptr<IMesh> _geom) { m_State.setGeometry(_geom); }
#endif
	uint32 getBlendMode() const { return m_WMOMaterial->getBlendMode(); }

	void Render();

private:
	const SWMO_Group_BatchDef	m_Proto;
	const WMO_Part_Material*	m_WMOMaterial;
	BoundingBox					m_Bounds;

	const WMO*					m_ParentWMO;
	const WMO_Group*			m_ParentGroup;
	const CGroupQuality&		m_Quality;
};

struct WMO_Group_Part_BatchCompare
{
	bool operator() (const WMO_Group_Part_Batch* left, const WMO_Group_Part_Batch* right) const
	{
		return left->getBlendMode() < right->getBlendMode();
	}
};