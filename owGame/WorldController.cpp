#include "stdafx.h"

// General
#include "WorldController.h"

WorldController* _World;

WorldController::WorldController()
{
	m_MapController = new MapController();
	m_MDXsManager = new CM2_Manager();
	m_WMOsManager = new WMOsManager(_Render->getAdapter());
	m_EnvironmentManager = new EnvironmentManager();
}

WorldController::~WorldController()
{
	delete m_MapController;
	delete m_MDXsManager;
	delete m_WMOsManager;
	delete m_EnvironmentManager;
}

void WorldController::EnterMap(vec3 _pos)
{
	m_MapController->EnterMap(_pos.x / C_TileSize, _pos.z / C_TileSize);
}

void WorldController::EnterMap(int32 x, int32 z)
{
	m_MapController->EnterMap(x, z);
}




