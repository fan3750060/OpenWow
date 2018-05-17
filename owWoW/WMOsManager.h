#pragma once

#include "Wmo.h"

class WMOsManager : public RefManager1DimAssync<WMO>
{
public:
	WMOsManager();
	~WMOsManager();

	WMO* CreateAction(cstring name) override;
	void LoadAction(string name, WMO*& _wmo) override;
	bool DeleteAction(cstring name) override;
};