#pragma once

#include "WMO_Group.h"

// FORWARD BEGIN
class CWMO_Base_Instance;
class CWMO_Doodad_Instance;
class CWMO_Liquid_Instance;
// FORWARD END

class CWMO_Group_Instance : public SceneNode
{
public:
	CWMO_Group_Instance(CWMO_Base_Instance* _parent, const WMO_Group* _object);
	~CWMO_Group_Instance();

	const WMO_Group* getObject() const { return m_Object; }

	void addDoodadInstance(std::shared_ptr<CWMO_Doodad_Instance> _doodad) { m_Doodads.push_back(_doodad); }
	const vector<std::shared_ptr<CWMO_Doodad_Instance>>& getDoodadsInstances() { return m_Doodads; }

	void addLiquidInstance(std::shared_ptr<CWMO_Liquid_Instance> _liquid) { m_Liquids.push_back(_liquid); }
	const vector<std::shared_ptr<CWMO_Liquid_Instance>>& getLiquidInstances() { return m_Liquids; }

	// Disable render
	bool PreRender3D() override;
	void Render3D() override;

//private:
	const WMO_Group*							m_Object;

	bool										m_PortalsVis;
	bool										m_Calculated;

	vector<std::shared_ptr<CWMO_Doodad_Instance>>		m_Doodads;
	vector<std::shared_ptr<CWMO_Liquid_Instance>>     m_Liquids;
};