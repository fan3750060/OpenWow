#include "stdafx.h"

// General
#include "WMOsManager.h"

WMOsManager::WMOsManager()
{
	AddManager<IWMOManager>(this);

	ADDCONSOLECOMMAND_CLASS("wmo_info", WMOsManager, PrintAllInfo);
}

std::shared_ptr<WMO> WMOsManager::CreateAction(cstring name)
{
	std::shared_ptr<WMO> _wmo = std::make_shared<WMO>(name);
	LoadAction(name, _wmo);
	return _wmo;
}

void WMOsManager::LoadAction(std::string name, std::shared_ptr<WMO>& item)
{
	if (!item->Load())
	{
		Log::Error("WMOsManager[%s]: Failed init WMO.", name.c_str());
	}
}

bool WMOsManager::DeleteAction(cstring name)
{
	return true;
}

void WMOsManager::MakeContext()
{

}
