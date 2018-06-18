#include "stdafx.h"

// General
#include "WMO_Liquid_Instance.h"

CWMO_Liquid_Instance::CWMO_Liquid_Instance(SceneNode* _parent, Liquid* _liquidObject, vec3 _position, const WMO_Group* _group) :
	Liquid_Instance(_parent, _liquidObject, _position),
	m_Group(_group)
{
}

void CWMO_Liquid_Instance::PreRender3D()
{
	setVisible(false);

	if (!m_Group->m_PortalsVis)
	{
		return;
	}

	setVisible(true);
}
