#include "stdafx.h"

// Include
#include "WMO.h"

// General
#include "WMO_Part_Portal.h"

CWMO_Part_Portal::CWMO_Part_Portal(const WMO* _parentWMO, const WMO_PortalDef& _proto) :
	m_ParentWMO(_parentWMO),
	m_Proto(_proto)
{
	m_Geom = _Render->r.beginCreatingGeometry(_Render->Storage()->__layout_GxVBF_P);
	m_Geom->setGeomVertexParams(m_ParentWMO->m_PortalVB, R_DataType::T_FLOAT, 0, sizeof(vec3));
	m_Geom->finishCreatingGeometry();
}

void CWMO_Part_Portal::Render()
{
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Render->TechniquesMgr()->m_Debug_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetPVW();
	_Render->TechniquesMgr()->m_Debug_GeometryPass->SetColor4(vec4(0.0f, 0.0f, 1.0f, 0.5f));

	_Render->r.setGeometry(m_Geom);
	_Render->r.draw(PRIM_TRISTRIP, m_Proto.startVertex, m_Proto.count);

	_Render->TechniquesMgr()->m_Debug_GeometryPass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
}
