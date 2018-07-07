#include "stdafx.h"

// Include
#include "WMO.h"
#include "WMO_Group.h"
#include "WMO_Base_Instance.h"

// General
#include "WMO_Part_Portal.h"

CWMO_Part_Portal::CWMO_Part_Portal(const WMO* _parentWMO, const SWMO_PortalDef& _proto) :
	m_ParentWMO(_parentWMO),
	m_GrInner(-1),
	m_GrOuter(-1)
{
	m_StartVertex = _proto.startVertex;
	m_Count = _proto.count;
	assert1(m_Count < 20);

	m_Plane.normal = _proto.plane.normal.toXZmY();
	m_Plane.dist = _proto.plane.distance;

	m_Geom = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_P);
	m_Geom->setGeomVertexParams(m_ParentWMO->m_PortalVB, R_DataType::T_FLOAT, 0, sizeof(vec3));
	m_Geom->finishCreatingGeometry();
}

void CWMO_Part_Portal::Render(cmat4 _worldMatrix)
{
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
	_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	{
		_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(_worldMatrix);

		vec4 color;
		if (m_Plane.distToPoint(_worldMatrix.inverted() * _Render->getCamera()->Position) > 0.0f)
			color = vec4(0.0f, 1.0f, 0.0f, 0.3f);
		else
			color = vec4(1.0f, 0.0f, 0.0f, 0.3f);

		/*if (IsVisible())
			color = vec4(0.0f, 1.0f, 0.0f, 0.3f);
		else
			color = vec4(1.0f, 0.0f, 0.0f, 0.3f);*/

		_Render->getTechniquesMgr()->Debug_Pass->SetColor4(color);

		_Render->r.setGeometry(m_Geom);
		_Render->r.draw(PRIM_TRISTRIP, m_StartVertex, m_Count);
	}
	_Render->getTechniquesMgr()->Debug_Pass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
}

void CWMO_Part_Portal::setGroup(int32 _group, int16 side)
{
	if (side == 1)
	{
		m_GrInner = _group;
	}
	else if (side == -1)
	{
		m_GrOuter = _group;
	}
	else
	{
		fail1();
	}
}

bool CWMO_Part_Portal::IsVisible(CWMO_Base_Instance* _localContr, const Plane* _planes, uint32 _planesCount) const
{
	if (_planes == nullptr || _planesCount == 0)
	{
		return false;
	}

	return !cullPolyByPlanes
	(
		_planes,
		_planesCount,
		_localContr->getVerts() + getStartVertex(),
		getCount()
	);
}

bool CWMO_Part_Portal::IsPositive(cvec3 _InvWorldCamera) const
{
	return m_Plane.distToPoint(_InvWorldCamera) > 0.0f;
}
