#include "stdafx.h"

// Include
#include "WMO.h"
#include "WMO_LocalPortalContainer.h"

// General
#include "WMO_Part_PortalsController.h"

// Additional
#include "WMO_Group.h"

uint32 ClipPlane(vec3* dst, const vec3* src, uint32 num_verts, const Plane plane)
{
	uint32 num = 0; // кол-во вершин в dst
	for (uint32 i = 0; i < num_verts; i++)
	{
		vec3 v1 = src[i];
		vec3 v2 = src[(i + 1) % num_verts];

		float t1 = v1.dot(plane.normal) + plane.dist;
		float t2 = v2.dot(plane.normal) + plane.dist;

		if (t1 >= 0)
		{
			dst[num] = v1;
			num++;
		}

		if (t1 * t2 < 0)
		{
			float k1 = t2 / (t2 - t1);
			float k2 = -t1 / (t2 - t1);

			dst[num].x = v1.x * k1 + v2.x * k2;
			dst[num].y = v1.y * k1 + v2.y * k2;
			dst[num].z = v1.z * k1 + v2.z * k2;
			num++;
		}
	}

	return num;
}

uint32 ClipPolygon(vec3* dst, const vec3* src, uint32 num_verts, const Plane* planes, uint32 num_planes)
{
	vec3 swap[50];

	uint32 num = ClipPlane(dst, src, num_verts, planes[0]);

	for (uint32 i = 1; i < num_planes; i++)
	{
		if (i % 2 != 0)  // проверка на чётность
		{
			num = ClipPlane(swap, dst, num, planes[i]);
		}
		else
		{
			num = ClipPlane(dst, swap, num, planes[i]);
		}
	}

	memcpy(dst, &swap, num * sizeof(vec3));

	return num;
}


CWMO_Part_PortalsController::CWMO_Part_PortalsController(const WMO * _parentWMO) :
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

		portal->setGroup(group, it.side);
	}
}

void CWMO_Part_PortalsController::GetPolyFrustum(const vec3* poly, uint32 num_verts, Plane* frustum, vec3 eye, bool _isPositive)
{
	for (uint32 i = 0; i < num_verts; i++)
	{
		vec3 v1 = poly[i];
		vec3 v2 = poly[(i + 1) % num_verts];

		if (_isPositive)
		{
			frustum[i] = Plane(eye, v1, v2);
		}
		else
		{
			frustum[i] = Plane(eye, v2, v1);
		}
	}
}

void CWMO_Part_PortalsController::Update(CWMO_LocalPortalContainer* _localContr, cvec3 _InvWorldCamera)
{
	for (auto& it : m_ParentWMO->m_Groups)
	{
		it->m_PortalsVis = false;
		it->m_Calculated = false;
	}

	const Plane* _viewPlanes = _Render->getCamera()->getFrustum().getPlanes();
	uint32 _viewPlanesCnt = 4;

	for (auto& group : m_ParentWMO->m_Groups)
	{
		if (!(group->isPointInside(_InvWorldCamera)))
		{
			continue;
		}

		Recur(group, _localContr, _InvWorldCamera, _viewPlanes, _viewPlanesCnt);
	}
}

void CWMO_Part_PortalsController::Recur(WMO_Group* _group, CWMO_LocalPortalContainer* _localContr, cvec3 _InvWorldCamera, const Plane* _planes, uint32 _planesCount)
{
	if (_group == nullptr || _group->m_Calculated)
	{
		return;
	}

	_group->m_PortalsVis = true;
	_group->m_Calculated = true;

	/*
		TODO: Check visible for MODD
	*/

	for (auto& p : _group->m_Portals)
	{
		// If we don't see portal // TODO: Don't use it on first step
		if (_Render->getCamera()->getFrustum().cullPoly(_localContr->getVerts() + p->getStartVertex(), p->getCount()))
		{
			continue;
		}

		// And we don't see portal from other portal
		if (!p->IsVisible(_localContr, _planes, _planesCount))
		{
			continue;
		}

		bool isPositive = p->IsPositive(_InvWorldCamera);

		Plane _portalPlanes[20];
		uint32 _portalPlanesCount = p->getCount();
		assert1(_portalPlanesCount < 20);
		GetPolyFrustum
		(
			_localContr->getVerts() + p->getStartVertex(),
			p->getCount(),
			_portalPlanes,
			_Render->getCamera()->Position,
			isPositive
		);

		Recur
		(
			isPositive ? p->getGrInner() : p->getGrOuter(),
			_localContr,
			_InvWorldCamera,
			_portalPlanes,
			_portalPlanesCount
		);
	}
}

/*bool CWMO_Part_PortalsController::IsVisible(CWMO_Part_Portal* _portal, CWMO_LocalPortalContainer* _localContr, cvec3 _InvWorldCamera, const Plane* _planes, uint32 _planesCount)
{
	// And we don't see portal from other portal
	if (!_portal->IsVisible(_localContr, _planes, _planesCount))
	{
		return false;
	}

	bool isPositive = _portal->IsPositive(_InvWorldCamera);

	Plane _portalPlanes[20];
	uint32 _portalPlanesCount = _portal->getCount();
	assert1(_portalPlanesCount < 20);
	GetPolyFrustum
	(
		_localContr->getVerts() + _portal->getStartVertex(),
		_portal->getCount(),
		_portalPlanes,
		_Render->getCamera()->Position,
		isPositive
	);

	Recur
	(
		isPositive ? _portal->getGrInner() : _portal->getGrOuter(),
		_localContr,
		_InvWorldCamera,
		_portalPlanes,
		_portalPlanesCount
	);

	return true;
}*/