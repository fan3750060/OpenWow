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
	CWMO_Group_Instance(std::weak_ptr<CWMO_Base_Instance> _parent, const WMO_Group* _object);
	~CWMO_Group_Instance();

	const WMO_Group* getObject() const { return m_Object; }

#ifdef GAME_WMO_INCLUDE_WM2
	void addDoodadInstance(std::shared_ptr<CWMO_Doodad_Instance> _doodad) { m_Doodads.push_back(_doodad); }
	const vector<std::shared_ptr<CWMO_Doodad_Instance>>& getDoodadsInstances() { return m_Doodads; }
#endif

	void addLiquidInstance(std::shared_ptr<CWMO_Liquid_Instance> _liquid) { m_Liquids.push_back(_liquid); }
	const std::vector<std::shared_ptr<CWMO_Liquid_Instance>>& getLiquidInstances() { return m_Liquids; }

	void Accept(IVisitor& visitor) override;

	// Disable render
	bool PreRender3D();
	void Render3D();

//private:
	const WMO_Group*							m_Object;

	bool										m_PortalsVis;
	bool										m_Calculated;

	std::vector<std::shared_ptr<CWMO_Doodad_Instance>>		m_Doodads;
	std::vector<std::shared_ptr<CWMO_Liquid_Instance>>     m_Liquids;
};