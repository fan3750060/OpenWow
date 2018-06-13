#include "stdafx.h"

// General
#include "WorldController.h"

WorldController::WorldController()
{
	m_MapController = new MapController();
	m_MDXsManager = new CM2_Manager();
	m_WMOsManager = new WMOsManager();
	m_EnvironmentManager = new EnvironmentManager();
}

WorldController::~WorldController()
{}

void WorldController::EnterMap(int32 x, int32 z)
{
	m_MapController->EnterMap(x, z);
}


