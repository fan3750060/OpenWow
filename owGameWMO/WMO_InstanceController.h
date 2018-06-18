#pragma once

class CWMO_InstanceController : public CRefItem
{
public:
	CWMO_InstanceController(const WMO* _parentWMO, cmat4 _world);

	cmat4 getWorld() const { return m_World; }
	cmat4 getInvWorld() const { return m_InvWorld; }
	const vec3* getVerts() const { return m_ConvertedVerts.data(); }

private:
	const WMO*					m_ParentWMO;
	const mat4					m_World;
	const mat4					m_InvWorld;
	vector<vec3>				m_ConvertedVerts;
};