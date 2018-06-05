#include "stdafx.h"

// General
#include "WMOsManager.h"

WMOsManager::WMOsManager()
{
	ADDCONSOLECOMMAND_CLASS("wmo_info", WMOsManager, PrintAllInfo);

	AddManager<IWMOManager>(this);
}

WMO* WMOsManager::CreateAction(cstring name)
{
	WMO* _wmo = new WMO(name);
	if (!_wmo->Load())
	{
		Log::Error("WMOsManager[%s]: Failed init WMO.", name.c_str());
	}
	return _wmo;
}

bool WMOsManager::DeleteAction(cstring name)
{
	return true;
}