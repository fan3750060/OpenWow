#pragma once

#include "MapController.h"
#include "MDXsManager.h"
#include "WMOsManager.h"
#include "EnvironmentManager.h"

class WorldController : UpdatableObject
{
	CLASS_INSTANCE(WorldController);
public:
	WorldController();
	~WorldController();

	void Update(double _Time, double _deltaTime);

	inline MapController* Map() { return m_MapController; }
	inline MDXsManager* MDXM() { return m_MDXsManager; }
	inline WMOsManager* WMOM() { return m_WMOsManager; }
	inline EnvironmentManager* EnvM() { return m_EnvironmentManager; }

	void EnterMap(int32 x, int32 z);
	MDX* LoadMDX(cstring name);
	WMO* LoadWMO(cstring name);

private:
	MapController*       m_MapController;
	MDXsManager*         m_MDXsManager;
	WMOsManager*         m_WMOsManager;
	EnvironmentManager*  m_EnvironmentManager;
};

#define _World WorldController::instance()