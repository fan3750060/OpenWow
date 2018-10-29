#pragma once

#include "WMO_Headers.h"

// FORWARD BEGIN
class CWMO_Base_Instance;
// FORWARD END

class CWMO_PortalsController
{
public:
	CWMO_PortalsController(const std::weak_ptr<const WMO> _parentWMO);

	void GetPolyFrustum(const vec3* poly, uint32 num_verts, Frustum* frustum, vec3 eye, bool _isPositive = true);
	
	void Update(CWMO_Base_Instance* _localContr, cvec3 _InvWorldCamera);
	bool Recur(CWMO_Base_Instance* _localContr, CWMO_Group_Instance* _group, cvec3 _InvWorldCamera, const Frustum& _frustum, bool _isFirstIteration);


private:
	const std::weak_ptr<const WMO> m_ParentWMO;
};