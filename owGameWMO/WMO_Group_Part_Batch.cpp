#include "stdafx.h"

// Include
#include "WMO.h"
#include "WMO_Group.h"

// General
#include "WMO_Group_Part_Batch.h"

WMO_Group_Part_Batch::WMO_Group_Part_Batch(const std::weak_ptr<const WMO> _parentWMO, const std::weak_ptr<const WMO_Group> _parentGroup, std::shared_ptr<IMesh> _mesh, const SWMO_Group_BatchDef& _proto) :
	MeshWrapper(_mesh),
	m_ParentWMO(_parentWMO),
	m_ParentGroup(_parentGroup),
	m_Proto(_proto),
	m_Quality(GetSettingsGroup<CGroupQuality>())
{
	m_WMOMaterial = m_ParentWMO.lock()->m_Materials[m_Proto.material_id];
	m_Bounds.setMin(vec3(_proto.bx, _proto.by, _proto.bz));
	m_Bounds.setMax(vec3(_proto.tx, _proto.ty, _proto.tz));
	m_Bounds.calculateCenter();

	SetMaterial(m_WMOMaterial);
	SetType(SN_TYPE_WMO);

	// Init renderState
	//m_WMOMaterial->fillRenderState(&m_State);
	//m_State.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
	//m_State.setDepthMask(true);
	//m_State.setDepthTest(true);
	//m_State.setGeometry(_parentGroup->__geom); geom is nullptr now, set it later
}

void WMO_Group_Part_Batch::Render(RenderEventArgs& renderEventArgs, std::shared_ptr<ConstantBuffer> perObject, UINT indexStartLocation, UINT indexCnt, INT baseVertexLocation)
{
	base::Render(renderEventArgs, perObject, m_Proto.indexStart, m_Proto.indexCount, m_Proto.vertexStart);

	/*CWMO_GeomertyPass* pass = _Render->getTechniquesMgr()->WMO_Pass.operator->();
	{
		pass->SetBlendMode(m_WMOMaterial->getBlendMode());

		_Render->r.drawIndexed
		(
			m_Proto.indexStart,
			m_Proto.indexCount,
			m_Proto.vertexStart,
			m_Proto.getVerticesCount(),
			&m_State,
			true
		);
	}*/
}
