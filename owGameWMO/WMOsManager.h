#pragma once

#include "WMO.h"

class WMOsManager : public IWMOManager, public CRefManager1Dim<WMO>
{
public:
	WMOsManager(IOpenGLAdapter* _adapter);

	// IWMOManager
	SmartWMOPtr Add(cstring name) { return CRefManager1Dim::Add(name); }
	bool Exists(cstring name) const { return CRefManager1Dim::Exists(name); }
	void Delete(cstring name) { CRefManager1Dim::Delete(name); }
	void Delete(SmartWMOPtr item) { CRefManager1Dim::Delete(item); }

	// CRefManager1Dim
	SmartWMOPtr CreateAction(cstring name) override;
	void LoadAction(std::string name, SmartWMOPtr& item);
	bool DeleteAction(cstring name) override;
	void MakeContext();

private:
	IOpenGLAdapter* m_Adapter;
};