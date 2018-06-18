#include "stdafx.h"

// Include
#include "WMO.h"

// General
#include "WMO_InstanceController.h"

CWMO_InstanceController::CWMO_InstanceController(const WMO* _parentWMO, cmat4 _world) :
	m_ParentWMO(_parentWMO),
	m_World(_world),
	m_InvWorld(_world.inverted())
{
	if (m_ParentWMO->m_PortalController != nullptr)
	{
		for (auto& v : m_ParentWMO->m_PortalVertices)
		{
			m_ConvertedVerts.push_back(_world * v);
		}
	}
}
