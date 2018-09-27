#include "stdafx.h"

// Include
#include "WMO.h"
#include "WMO_Base_Instance.h"

// General
#include "WMO_PortalsController.h"

// Additional
#include "WMO_Group_Instance.h"

CWMO_PortalsController::CWMO_PortalsController(const WMO* _parentWMO) :
	m_ParentWMO(_parentWMO)
{
	for (auto& it : m_ParentWMO->m_PortalReferences)
	{
		CWMO_Part_Portal* portal = m_ParentWMO->m_Portals[it.portalIndex];
		WMO_Group* group = m_ParentWMO->m_Groups[it.groupIndex];

		if (it.groupIndex == group->m_GroupIndex)
		{
			group->m_Portals.push_back(portal);
		}
		else
		{
			fail1();
		}

		portal->setGroup(it.groupIndex, it.side);
	}
}

void CWMO_PortalsController::GetPolyFrustum(const vec3* poly, uint32 num_verts, Frustum* _frustum, vec3 eye, bool _isPositive)
{
	_ASSERT(_frustum != nullptr);

	Plane _portalPlanes[15];
	_ASSERT(num_verts < 15);

	for (uint32 i = 0; i < num_verts; i++)
	{
		vec3 v1 = poly[i];
		vec3 v2 = poly[(i + 1) % num_verts];

		if (_isPositive)
		{
			_portalPlanes[i] = Plane(eye, v1, v2);
		}
		else
		{
			_portalPlanes[i] = Plane(eye, v2, v1);
		}
	}

	_frustum->buildCustomFrustrum(_portalPlanes, num_verts);
}

void CWMO_PortalsController::Update(CWMO_Base_Instance* _localContr, cvec3 _InvWorldCamera)
{
	// Reset all flags
	for (auto& group : _localContr->getGroupInstances())
	{
		group->m_PortalsVis = false;
		group->m_Calculated = false;
		for (auto& doodad : group->getDoodadsInstances())
		{
			if (doodad)
			{
				doodad->setPortalVisibility(false);
			}
		}
	}

	bool insideIndoor = false;

	if (m_ParentWMO->getBounds().isPointInside(_InvWorldCamera))
	{
		for (auto& group : _localContr->getGroupInstances())
		{
			if (!(group->getBounds().isPointInside(_Render->getCamera()->Position)))
			{
				continue;
			}

			if (!group->getObject()->m_Header.flags.HAS_COLLISION)
			{
				continue;
			}

			if (group->getObject()->m_Header.flags.IS_OUTDOOR)
			{
				continue;
			}

			bool recurResult = Recur(_localContr, group.operator->(), _InvWorldCamera, _Render->getCamera()->getFrustum(), true);
			if (!recurResult)
			{
				continue;
			}

			if (group->getObject()->m_Header.flags.IS_INDOOR)
			{
				insideIndoor = true;
			}
		}
	}

	//_ASSERT(insideOneAtLeast || !(m_ParentWMO->m_OutdoorGroups.empty()));

	// If we outside WMO, then get outdorr group
	//if (!insideIndoor)
	{
		for (auto& ogr : _localContr->getGroupOutdoorInstances())
		{
			Recur(_localContr, ogr.operator->(), _InvWorldCamera, _Render->getCamera()->getFrustum(), true);
		}
	}
}

bool CWMO_PortalsController::Recur(CWMO_Base_Instance* _localContr, CWMO_Group_Instance* _group, cvec3 _InvWorldCamera, const Frustum& _frustum, bool _isFirstIteration)
{
	if (_group == nullptr || _group->m_Calculated)
	{
		return false;
	}

	if (_Render->getCamera()->getFrustum().cullBox(_group->getBounds()))
	{
		return false;
	}

	// Set visible for current
	_group->m_PortalsVis = true;
	_group->m_Calculated = true;
	for (auto& doodad : _group->getDoodadsInstances())
	{
		if (doodad && (_isFirstIteration || !_frustum.cullBox(doodad->getBounds())))
		{
			doodad->setPortalVisibility(true);
		}
	}

	for (auto& p : _group->getObject()->m_Portals)
	{
		// If we don't see portal // TODO: Don't use it on first step
		if (_Render->getCamera()->getFrustum().cullPoly(_localContr->getVerts() + p->getStartVertex(), p->getCount()))
		{
			continue;
		}

		// And we don't see portal from other portal
		if (!p->IsVisible(_localContr, _frustum.getPlanes(), _frustum.getPlanesCnt()))
		{
			continue;
		}

		bool isPositive = p->IsPositive(_InvWorldCamera);

		// Build camera-to-poratl frustum
		Frustum portalFrustum;
		GetPolyFrustum
		(
			_localContr->getVerts() + p->getStartVertex(),
			p->getCount(),
			&portalFrustum,
			_Render->getCamera()->Position,
			isPositive
		);

		// Find attached to portal group
		CWMO_Group_Instance* groupInstance = nullptr;
		int32 groupIndex = isPositive ? p->getGrInner() : p->getGrOuter();
		if (groupIndex >= 0 && groupIndex < _localContr->getGroupInstances().size())
		{
			groupInstance = _localContr->getGroupInstances()[groupIndex].operator->();
		}

		Recur
		(
			_localContr,
			groupInstance,
			_InvWorldCamera,
			portalFrustum,
			false
		);
	}

	return true;
}