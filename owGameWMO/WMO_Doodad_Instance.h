#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Doodad_Instance : public CM2_Base_Instance
{
public:
	CWMO_Doodad_Instance(SceneNode* _parent, M2* _mdxObject, uint32 _index, const WMO_Doodad_PlacementInfo& _placement, const WMO_Group* _group);
	~CWMO_Doodad_Instance();

	// ISceneNode
	string getObjectInfo() override { return m_Object->getFilename(); }

	// IRenderable
	bool PreRender3D() override;
	void Render3D() override;

private:
	const uint32		m_Index;
	const WMO_Group*	m_Group;					// used for visibility
};