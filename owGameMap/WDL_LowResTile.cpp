#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "WDL_LowResTile.h"

CWDL_LowResTile::CWDL_LowResTile(std::weak_ptr<MapController> _parent, uint32 _indexX, uint32 _indexZ, SharedMeshPtr _geom) :
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
		//setOpaque(true);
		//setDrawOrder(19);
	}

	AddMesh(m_Geom);

	// State
	//m_State.setDepthTest(false);
	//m_State.setDepthMask(false);
	//_Render->getRenderStorage()->SetEGxBlend(&m_State, 0);
	//m_State.setGeometry(m_Geom);
}

void CWDL_LowResTile::Render(RenderEventArgs& renderEventArgs)
{
	std::shared_ptr<MapController> mapController = m_MapController.lock();
	_ASSERT(mapController != NULL);

	int32 currentX = mapController->GetCurrentX();
	int32 currentZ = mapController->GetCurrentZ();

	/*if (m_MapController->getTileIsCurrent(m_IndexX, m_IndexZ))
	{
		return;
	}*/

	if (abs(m_IndexX - currentX) > 7 || abs(m_IndexZ - currentZ) > 7)
	{
		return;
	}

	SceneNode::Render(renderEventArgs);
}
