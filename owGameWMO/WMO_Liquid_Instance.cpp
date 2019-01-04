#include "stdafx.h"

// General
#include "WMO_Liquid_Instance.h"

CWMO_Liquid_Instance::CWMO_Liquid_Instance(std::shared_ptr<Liquid> _liquidObject, vec3 _position, const std::weak_ptr<const WMO_Group> _group) :
	Liquid_Instance(_liquidObject, _position),
	m_Group(_group)
{
}

bool CWMO_Liquid_Instance::PreRender3D()
{
	/*if (!m_Group->m_PortalsVis)
	{
		return false;
	}*/

	return true;
}
