#pragma once

#include "MapController.h"
#include "WMOsManager.h"
#include "EnvironmentManager.h"

class WorldController
{
	CLASS_INSTANCE(WorldController);
public:
	WorldController();
	~WorldController();

	inline MapController* Map() { return m_MapController; }
	inline EnvironmentManager* EnvM() { return m_EnvironmentManager; }

	void EnterMap(int32 x, int32 z);
	M2* LoadMDX(cstring name);
	WMO* LoadWMO(cstring name);

private:
	MapController*       m_MapController;
	CM2_Manager*         m_MDXsManager;
	WMOsManager*         m_WMOsManager;
	EnvironmentManager*  m_EnvironmentManager;
};

#define _World WorldController::instance()