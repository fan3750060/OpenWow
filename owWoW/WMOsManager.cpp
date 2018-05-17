#include "stdafx.h"

// Includes

// General
#include "WMOsManager.h"

// Additional
#include "Wmo.h"

WMOsManager::WMOsManager()
{
	ADDCONSOLECOMMAND_CLASS("wmo_info", WMOsManager, PrintAllInfo);

	RefManager1DimAssync::Init();
}

WMOsManager::~WMOsManager()
{
	RefManager1DimAssync::Destroy();

	DeleteAll();

	Log::Info("WMOsManager[]: All WMOs destroyed.");
}

WMO* WMOsManager::CreateAction(cstring name)
{
	WMO* _wmo = new WMO(name);
	return _wmo;
}

void WMOsManager::LoadAction(string name, WMO*& _wmo)
{
	//wglMakeCurrent(_Render->dc, _Render->glrc4);

	if (!_wmo->Load())
	{
		Log::Error("WMOsManager[%s]: Failed init WMO.", name.c_str());
	}
}

bool WMOsManager::DeleteAction(cstring name)
{
	return true;
}