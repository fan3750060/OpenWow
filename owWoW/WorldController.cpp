#include "stdafx.h"

// General
#include "WorldController.h"

WorldController::WorldController()
{
	m_MapController = new MapController();
	m_MDXsManager = new MDXsManager();
	m_WMOsManager = new WMOsManager();
	m_EnvironmentManager = new EnvironmentManager();

	UpdatableObject::Register();
}

WorldController::~WorldController()
{
	UpdatableObject::Unregister();

	delete m_MapController;
	delete m_WMOsManager;
	delete m_MDXsManager;
}

void WorldController::Update(double _Time, double _deltaTime)
{
	m_EnvironmentManager->animtime += (_deltaTime * 1000.0f);
	m_EnvironmentManager->globalTime = static_cast<int>(m_EnvironmentManager->animtime);

	_Config.Distances.Update();

	m_MapController->Tick();

	while (_deltaTime > 0.1f)
	{
		m_MDXsManager->updateEmitters(0.1f);
		_deltaTime -= 0.1f;
	}

	m_MDXsManager->updateEmitters(_deltaTime);
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


