#include "stdafx.h"

// Include
#include "WMO.h"

// General
#include "WMO_Group_Part_Batch.h"

WMO_Group_Part_Batch::WMO_Group_Part_Batch(const WMO* _parentWMO, const WMO_Group_BatchDef& _proto) :
	m_ParentWMO(_parentWMO),
	m_Proto(_proto),
	m_Quality(GetSettingsGroup<CGroupQuality>())
{
	m_WMOMaterial = m_ParentWMO->m_Materials[m_Proto.material_id];

	__material.SetDiffuseTexture(m_WMOMaterial->texture);
	__material.SetBlendState(m_WMOMaterial->GetBlendMode());
	__material.SetRenderState(m_WMOMaterial->IsTwoSided());
}

void WMO_Group_Part_Batch::Render()
{
	__material.Set();

	_Render->r.drawIndexed(PRIM_TRILIST, m_Proto.indexStart, m_Proto.indexCount, m_Proto.vertexStart, m_Proto.getVerticesCount(), false);
}
