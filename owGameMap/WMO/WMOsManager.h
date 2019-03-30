#pragma once

#include "WMO.h"

class WMOsManager : public IWMOManager, public CRefManager1Dim<WMO>
{
public:
	WMOsManager();

	// IWMOManager
	std::shared_ptr<WMO> Add(const std::string& name) { return CRefManager1Dim::Add(name); }
	bool Exists(const std::string& name) const { return CRefManager1Dim::Exists(name); }
	void Delete(const std::string& name) { CRefManager1Dim::Delete(name); }
	void Delete(std::shared_ptr<WMO> item) { CRefManager1Dim::Delete(item); }

	// CRefManager1Dim
	std::shared_ptr<WMO> CreateAction(const std::string& name) override;
	void LoadAction(std::string name, std::shared_ptr<WMO>& item);
	bool DeleteAction(const std::string& name) override;
	void MakeContext();
};