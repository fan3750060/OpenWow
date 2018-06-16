#include "stdafx.h"

// Include
#include "WMO.h"

// General
#include "WMO_Part_Portal.h"

CWMO_Part_Portal::CWMO_Part_Portal(const WMO* _parentWMO, const WMO_PortalDef& _proto) :
	m_ParentWMO(_parentWMO),
	m_Proto(_proto)
{
	m_Geom = _Render->r.beginCreatingGeometry(_Render->getRenderStorage()->__layout_GxVBF_P);
	m_Geom->setGeomVertexParams(m_ParentWMO->m_PortalVB, R_DataType::T_FLOAT, 0, sizeof(vec3));
	m_Geom->finishCreatingGeometry();
}

void CWMO_Part_Portal::Render(cmat4 _worldMatrix)
{
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Render->getTechniquesMgr()->Debug_Pass->Bind();
	_Render->getTechniquesMgr()->Debug_Pass->SetWorldMatrix(_worldMatrix);
	_Render->getTechniquesMgr()->Debug_Pass->SetColor4(vec4(0.0f, 0.0f, 1.0f, 0.5f));

	_Render->r.setGeometry(m_Geom);
	_Render->r.draw(PRIM_TRISTRIP, m_Proto.startVertex, m_Proto.count);

	_Render->getTechniquesMgr()->Debug_Pass->Unbind();

	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
}
