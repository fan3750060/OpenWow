#pragma once

#include "Wmo.h"

class WMOsManager : public IWMOManager, public CRefManager1Dim<WMO>
{
public:
	WMOsManager();

	// IWMOManager
	WMO* Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(WMO* item) { CRefManager1Dim::Delete(item); }

	// CRefManager1Dim
	WMO* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;
};