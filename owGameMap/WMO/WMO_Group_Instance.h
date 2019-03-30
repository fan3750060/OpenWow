#pragma once

#include "WMO_Group.h"

// FORWARD BEGIN
class CWMO_Base_Instance;
class CWMO_Doodad_Instance;
class CWMO_Liquid_Instance;
// FORWARD END

class CWMO_Group_Instance : public SceneNodeModel3D
{
public:
	typedef std::vector<std::shared_ptr<CWMO_Doodad_Instance>> DoodadInstances;
	typedef std::vector<std::shared_ptr<CWMO_Liquid_Instance>> LuqidInstances;
public:
	CWMO_Group_Instance(const std::shared_ptr<WMO_Group> _object);
	virtual ~CWMO_Group_Instance();

	// SceneNode3D
	void SetParent(std::weak_ptr<SceneNode3D> pNode) override;
	bool Accept(IVisitor& visitor) override;

	const std::shared_ptr<WMO_Group> getObject() const { return m_Object; }

	void addDoodadInstance(std::shared_ptr<CWMO_Doodad_Instance> _doodad) { m_Doodads.push_back(_doodad); }
	const DoodadInstances& getDoodadsInstances() { return m_Doodads; }

	void addLiquidInstance(std::shared_ptr<CWMO_Liquid_Instance> _liquid) { m_Liquids.push_back(_liquid); }
	const LuqidInstances& getLiquidInstances() { return m_Liquids; }



//private:
	const std::shared_ptr<WMO_Group> m_Object;

	bool                             m_PortalsVis;
	bool                             m_Calculated;

	DoodadInstances                  m_Doodads;
	LuqidInstances                   m_Liquids;
};