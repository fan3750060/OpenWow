#include "stdafx.h"

// General
#include "WMOsManager.h"

WMOsManager::WMOsManager(IOpenGLAdapter* _adapter) :
	m_Adapter(_adapter)
{
	AddManager<IWMOManager>(this);

	ADDCONSOLECOMMAND_CLASS("wmo_info", WMOsManager, PrintAllInfo);
}

SmartWMOPtr WMOsManager::CreateAction(cstring name)
{
	SmartWMOPtr _wmo = std::make_shared<WMO>(name);
	LoadAction(name, _wmo);
	return _wmo;
}

void WMOsManager::LoadAction(std::string name, SmartWMOPtr& item)
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
	m_Adapter->MakeThreadContext();
}
