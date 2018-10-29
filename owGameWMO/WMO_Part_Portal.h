#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO;
class WMO_Group;
class CWMO_Base_Instance;
// FORWARD END

class CWMO_Part_Portal
{
public:
	CWMO_Part_Portal(const std::weak_ptr<const WMO> _parentWMO, const SWMO_PortalDef& _proto);

	void Render(cmat4 _worldMatrix);

	uint16 getStartVertex() const { return m_StartVertex; }
	uint16 getCount() const { return m_Count; }
	const Plane& getPlane() const { return m_Plane; }

	void setGroup(int32 _group, int16 side);
	int32 getGrInner() { return m_GrInner; }
	int32 getGrOuter() { return m_GrOuter; }

	bool IsVisible(CWMO_Base_Instance* _localContr, const Plane* _planes, uint32 _planesCount) const;
	bool IsPositive(cvec3 _InvWorldCamera) const;

private:
	const std::weak_ptr<const WMO>				m_ParentWMO;
	BoundingBox				m_Bounds;

	uint16					m_StartVertex;
	uint16					m_Count;
	Plane					m_Plane;

	int32					m_GrInner;
	int32					m_GrOuter;

	std::shared_ptr<IMesh>			m_Geom;
};