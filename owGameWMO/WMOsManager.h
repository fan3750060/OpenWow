#pragma once

#include "WMO.h"

class WMOsManager : public IWMOManager, public CRefManager1Dim<WMO>
{
public:
	WMOsManager(IOpenGLAdapter* _adapter);

	// IWMOManager
	WMO* Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(WMO* item) { CRefManager1Dim::Delete(item); }

	// CRefManager1Dim
	WMO* CreateAction(cstring name) override;
	void LoadAction(string name, WMO*& item);
	bool DeleteAction(cstring name) override;
	void MakeContext();

private:
	IOpenGLAdapter* m_Adapter;
};