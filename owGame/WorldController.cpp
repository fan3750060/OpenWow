#include "stdafx.h"

// General
#include "WorldController.h"

// Additional
#include "Loader.h"

WorldController::WorldController()
{
	new CLoader();

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

M2* WorldController::LoadMDX(cstring name)
{
	return m_MDXsManager->Add(name);
}

WMO* WorldController::LoadWMO(cstring name)
{
	return m_WMOsManager->Add(name);
}


