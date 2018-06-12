#include "stdafx.h"

// General
#include "WDL_LowResTile.h"

// Additional
#include "WorldController.h"

CWDL_LowResTile::CWDL_LowResTile(SceneNode* _parent, uint32 _indexX, uint32 _indexZ, R_GeometryInfo* _geom) :
	SceneNode(_parent),
	m_IndexX(_indexX), 
	m_IndexZ(_indexZ),
	m_Geom(_geom)
{
	setDrawOrder(19);
}

void CWDL_LowResTile::PreRender3D()
{
	setVisible(true);
}

void CWDL_LowResTile::Render3D()
{
	PERF_START(PERF_MAP_LOWRESOLUTION);
	//---------------------------------

	_Render->r.setDepthTest(false);
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
	//_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	_Pipeline->Clear();

	_Render->TechniquesMgr()->m_MapTileLowRes_GeometryPass->Bind();
	_Render->TechniquesMgr()->m_MapTileLowRes_GeometryPass->SetPV();
	_Render->TechniquesMgr()->m_MapTileLowRes_GeometryPass->SetShadowColor(/*vec3(1.0, 0.0, 0.0)*/_World->EnvM()->m_SkyManager->GetColor(LIGHT_COLOR_FOG));

	_Render->r.setGeometry(m_Geom);
	_Render->r.draw(PRIM_TRILIST, 0, 16 * 16 * 12);
	PERF_INC(PERF_MAP_LOWRESOLUTION);

	_Render->TechniquesMgr()->m_MapTileLowRes_GeometryPass->Unbind();

	//_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
	_Render->r.setDepthTest(true);

	//---------------------------------
	PERF_STOP(PERF_MAP_LOWRESOLUTION);
}
