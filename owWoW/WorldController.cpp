#include "stdafx.h"

// General
#include "WorldController.h"

// Additional
#include "Loader.h"

WorldController::WorldController()
{
	new CLoader();

	m_MapController = new MapController();
	m_MDXsManager = new MDXsManager();
	m_WMOsManager = new WMOsManager();
	m_EnvironmentManager = new EnvironmentManager();

	_Bindings->RegisterUpdatableObject(this);
}

WorldController::~WorldController()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void WorldController::Update(double _Time, double _deltaTime)
{
}

void WorldController::EnterMap(int32 x, int32 z)
{
	m_MapController->EnterMap(x, z);
}

MDX* WorldController::LoadMDX(cstring name)
{
	return m_MDXsManager->Add(name);
}

WMO* WorldController::LoadWMO(cstring name)
{
	return m_WMOsManager->Add(name);
}


