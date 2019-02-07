#pragma once

#include "WMO_Headers.h"

#include "WMO_Group.h"

class CWMO_Doodad_Instance : public CM2_Base_Instance
{
public:
	CWMO_Doodad_Instance(std::string _m2Name, const std::weak_ptr<const WMO_Group> _parentGroup, uint32 _index, const SWMO_Doodad_PlacementInfo& _placement);
	virtual ~CWMO_Doodad_Instance();

	void setPortalVisibility(bool _visibility) { m_PortalVis = _visibility; }
		
	// SceneNode3D
	bool Accept(IVisitor& visitor) override;

	// IRenderable
	bool PreRender3D();
	void Render3D();

private:
	const std::weak_ptr<const WMO_Group>	m_ParentGroup;
	const uint32		m_Index;
	bool				m_PortalVis;
};
