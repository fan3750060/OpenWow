#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Doodad_Instance : public CM2_Base_Instance
{
public:
	CWMO_Doodad_Instance(SceneNode* _parent, M2* _mdxObject, const WMO_Group* _parentGroup, uint32 _index, const SWMO_Doodad_PlacementInfo& _placement);
	~CWMO_Doodad_Instance();

	void setPortalVisibility(bool _visibility) { m_PortalVis = _visibility; }
		
	// ISceneNode
	string getObjectInfo() override { return m_Object->getFilename(); }

	// IRenderable
	bool PreRender3D() override;
	void Render3D() override;

private:
	const WMO_Group*	m_ParentGroup;
	const uint32		m_Index;
	bool				m_PortalVis;
};