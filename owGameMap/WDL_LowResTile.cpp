#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "WDL_LowResTile.h"

CWDL_LowResTile::CWDL_LowResTile(MapController* _parent, uint32 _indexX, uint32 _indexZ, SharedGeomPtr _geom) :
	SceneNode(_parent),
	m_MapController(_parent),
	m_IndexX(_indexX),
	m_IndexZ(_indexZ),
	m_Geom(_geom)
{
	{
		BoundingBox bbox; // Infinity
		bbox.calculateCenter();
		setBounds(bbox);

		// Scene node settings
		setOpaque(true);
		setDrawOrder(19);
	}

	// State
	m_State.setDepthTest(false);
	m_State.setDepthMask(false);
	_Render->getRenderStorage()->SetEGxBlend(&m_State, 0);
	m_State.setGeometry(m_Geom);
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
		CWDL_LowRes_Pass* pass = _Render->getTechniquesMgr()->WDL_LowRes_Pass.operator->();
		pass->Bind();
		{
			pass->setColor(m_MapController->getSkyManager()->GetColor(LightColors::LIGHT_COLOR_FOG));

			_Render->r.draw(0, 16 * 16 * 12, &m_State);
			PERF_INC(PERF_MAP_LOWRESOLUTION);
		}
		pass->Unbind();
	}
	PERF_STOP(PERF_MAP_LOWRESOLUTION);
}
