#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class WMO_Group;
class CWMO_InstanceController;
// FORWARD END

class CWMO_Part_Portal
{
public:
	CWMO_Part_Portal(const WMO* _parentWMO, const WMO_PortalDef& _proto);

	void Render(cmat4 _worldMatrix);

	uint16 getStartVertex() const { return m_StartVertex; }
	uint16 getCount() const { return m_Count; }
	const Plane& getPlane() const { return m_Plane; }

	void setGroup(WMO_Group* _group, int16 side);
	WMO_Group* getGrInner() { return m_GrInner; }
	WMO_Group* getGrOuter() { return m_GrOuter; }

	bool IsVisible(CWMO_InstanceController* _localContr, const Plane* _planes, uint32 _planesCount) const;
	bool IsPositive(cvec3 _InvWorldCamera) const;

private:
	const WMO*				m_ParentWMO;
	BoundingBox				m_Bounds;

	uint16					m_StartVertex;
	uint16					m_Count;
	Plane					m_Plane;

	WMO_Group*				m_GrInner;
	WMO_Group*				m_GrOuter;

	SmartGeomPtr			m_Geom;
};