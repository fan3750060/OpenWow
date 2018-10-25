#pragma once

#ifdef GAME_WMO_INCLUDE_WM2

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Doodad_Instance : public CM2_Base_Instance
{
public:
	CWMO_Doodad_Instance(SceneNode* _parent, SmartM2Ptr _mdxObject, const WMO_Group* _parentGroup, uint32 _index, const SWMO_Doodad_PlacementInfo& _placement);
	virtual ~CWMO_Doodad_Instance();

	void setPortalVisibility(bool _visibility) { m_PortalVis = _visibility; }
		
	// ISceneNode
	std::string getObjectInfo() const override { return getM2()->getFilename(); }

	// IRenderable
	bool PreRender3D() override;
	void Render3D() override;

private:
	const WMO_Group*	m_ParentGroup;
	const uint32		m_Index;
	bool				m_PortalVis;
};

#endif
