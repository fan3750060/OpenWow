#pragma once

#include "WMO_Group.h"

// FORWARD BEGIN
class CWMO_Base_Instance;
class CWMO_Doodad_Instance;
// FORWARD END

class CWMO_Group_Instance : public SceneNode
{
public:
	CWMO_Group_Instance(CWMO_Base_Instance* _parent, const WMO_Group* _object);
	~CWMO_Group_Instance();

	const WMO_Group* getObject() const { return m_Object; }

	void addDoodadInstance(CWMO_Doodad_Instance* _doodad) { m_Doodads.push_back(_doodad); }
	const vector<CWMO_Doodad_Instance*>& getDoodadsInstances() { return m_Doodads; }

	// Disable render
	bool PreRender3D() override;
	void Render3D() override;

//private:
	const WMO_Group*						m_Object;

	bool									m_PortalsVis;
	bool									m_Calculated;

	vector<CWMO_Doodad_Instance*>			m_Doodads;
};