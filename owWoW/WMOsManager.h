#pragma once

#include "Wmo.h"

class WMOsManager : public IWMOManager, public RefManager1Dim<WMO>
{
public:
	WMOsManager();
	~WMOsManager();

	// IWMOManager
	WMO* Add(cstring name) { return RefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return RefManager1Dim::Exists(name); }
	void Delete(cstring name) { RefManager1Dim::Delete(name); }
	void Delete(WMO* item) { RefManager1Dim::Delete(item); }

	// RefManager1Dim
	WMO* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;
};