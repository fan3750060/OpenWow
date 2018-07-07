#include "stdafx.h"

// Include
#include "WMO.h"

// General
#include "WMO_Group_Part_Batch.h"

WMO_Group_Part_Batch::WMO_Group_Part_Batch(const WMO* _parentWMO, const SWMO_Group_BatchDef& _proto) :
	m_ParentWMO(_parentWMO),
	m_Proto(_proto),
	m_Quality(GetSettingsGroup<CGroupQuality>())
{
	m_WMOMaterial = m_ParentWMO->m_Materials[m_Proto.material_id];
	m_Bounds.setMin(vec3(_proto.bx, _proto.by, _proto.bz));
	m_Bounds.setMax(vec3(_proto.tx, _proto.ty, _proto.tz));
	m_Bounds.calculateCenter();
}

void WMO_Group_Part_Batch::Render()
{
	m_WMOMaterial->set();

	CWMO_GeomertyPass* pass = _Render->getTechniquesMgr()->m_WMO_GeometryPass;
	{
		pass->SetBlendMode(m_WMOMaterial->getBlendMode());

		_Render->r.drawIndexed
		(
			PRIM_TRILIST,
			m_Proto.indexStart,
			m_Proto.indexCount,
			m_Proto.vertexStart,
			m_Proto.getVerticesCount(),
			false
		);
	}
}
