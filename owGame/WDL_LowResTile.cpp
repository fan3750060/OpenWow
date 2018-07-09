#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "WDL_LowResTile.h"

// Additional
#include "WorldController.h"

CWDL_LowResTile::CWDL_LowResTile(SceneNode* _parent, uint32 _indexX, uint32 _indexZ, R_GeometryInfo* _geom) :
	SceneNode(_parent),
	m_MapController(dynamic_cast<MapController*>(_parent)),
	m_IndexX(_indexX),
	m_IndexZ(_indexZ),
	m_Geom(_geom)
{
	m_Bounds.calculateCenter();

	setOpaque(true);
	setDrawOrder(19);

	m_Context.depthTest = false;
}

bool CWDL_LowResTile::PreRender3D()
{
	int32 currentX = m_MapController->GetCurrentX();
	int32 currentZ = m_MapController->GetCurrentZ();

	/*if (m_MapController->getTileIsCurrent(m_IndexX, m_IndexZ))
	{
		return;
	}*/

	if (abs(m_IndexX - currentX) > 7 || abs(m_IndexZ - currentZ) > 7)
	{
		return false;
	}

	return true;
}

void CWDL_LowResTile::Render3D()
{
	PERF_START(PERF_MAP_LOWRESOLUTION);
	{
		_Render->r.setDepthTest(false);
		_Render->r.setDepthMask(false);
		_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
		//_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
		_Render->getRenderStorage()->SetEGxBlend(_Render->r.getState(), 0);

		_Render->r.setGeometry(m_Geom);

		CWDL_LowRes_Pass* pass = _Render->getTechniquesMgr()->WDL_LowRes_Pass;
		pass->Bind();
		{
			pass->setColor(_World->EnvM()->m_SkyManager->GetColor(LightColors::LIGHT_COLOR_FOG));

			_Render->r.draw(PRIM_TRILIST, 0, 16 * 16 * 12);
			PERF_INC(PERF_MAP_LOWRESOLUTION);
		}
		pass->Unbind();

		//_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
		_Render->r.setCullMode(R_CullMode::RS_CULL_NONE);
		_Render->r.setDepthMask(true);
		_Render->r.setDepthTest(true);

	}
	PERF_STOP(PERF_MAP_LOWRESOLUTION);
}
