#pragma once

#include "Wmo.h"

class WMOsManager : public RefManager1Dim<WMO>
{
public:
	WMOsManager();
	~WMOsManager();

	WMO* CreateAction(cstring name) override;
	bool DeleteAction(cstring name) override;
};