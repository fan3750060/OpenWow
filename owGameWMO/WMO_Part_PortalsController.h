#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class CWMO_InstanceController;
// FORWARD END

class CWMO_Part_PortalsController
{
public:
	CWMO_Part_PortalsController(const WMO* _parentWMO);

	void GetPolyFrustum(const vec3* poly, uint32 num_verts, Plane* frustum, vec3 eye, bool _isPositive = true);
	
	void Update(CWMO_InstanceController* _localContr, cvec3 _InvWorldCamera);
	void Recur(WMO_Group* _group, CWMO_InstanceController* _localContr, cvec3 _InvWorldCamera, const Plane* _planes, uint32 _planesCount);
	//bool IsVisible(CWMO_Part_Portal* _portal, CWMO_InstanceController* _localContr, cvec3 _InvWorldCamera, const Plane* _planes, uint32 _planesCount);

private:
	const WMO* m_ParentWMO;
};