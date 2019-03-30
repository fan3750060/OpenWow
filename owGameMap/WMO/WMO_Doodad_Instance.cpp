#include "stdafx.h"

// General
#include "WMO_Doodad_Instance.h"

CWMO_Doodad_Instance::CWMO_Doodad_Instance(std::string _m2Name, const std::weak_ptr<const WMO_Group> _parentGroup, uint32 _index, const SWMO_Doodad_PlacementInfo& _placement) :
	CM2_Base_Instance(_m2Name),
	m_ParentGroup(_parentGroup),
	m_Index(_index),
	m_PortalVis(true)
{
	/*if (_mdxObject->getFilename().find("LD_LIGHTSHAFT") != -1)
	{
		Log::Green("MODEL [%s] contains color [%f, %f, %f, %f]", _mdxObject->getFilename().c_str(), _placement.getColor().x, _placement.getColor().y, _placement.getColor().z, _placement.getColor().w);
	}*/

	//if (!m_ParentGroup->m_Header.flags.IS_OUTDOOR)
	{
		//m_DoodadColor = _placement.getColor();
	}
	//else
	{
		//m_DoodadColor = vec4(1.0f);
	}
	//m_Object->setDoodadColor(_placement.getColor());

	// Scene node params
	{
		SetTranslate(Fix_XZmY(_placement.position));
		SetRotationQuaternion(quat(_placement.orientation.w, -_placement.orientation.z, _placement.orientation.x, _placement.orientation.y));
		SetScale(vec3(_placement.scale, -_placement.scale, -_placement.scale));
	}
}

CWMO_Doodad_Instance::~CWMO_Doodad_Instance()
{
	//Log::Info("ADT_MDX Deleted");
}

bool CWMO_Doodad_Instance::Accept(IVisitor& visitor)
{
	if (!m_PortalVis)
	{
		return false;
	}

	return CM2_Base_Instance::Accept(visitor);
}

bool CWMO_Doodad_Instance::PreRender3D()
{
	if (!m_PortalVis)
	{
		return false;
	}

	// Check distance to camera
	//float distToCamera = glm::length(_Render->getCamera()->Position - GetBounds().getCenter()) - GetBounds().getRadius();
	//if (distToCamera > m_QualitySettings.WMO_MODD_Distance)
	//{
	//	return false;
	//}

	// Frustrum culling
	//if (!CM2_Base_Instance::PreRender3D())
	//{
	//	return false;
	//}

	return true;
}

void CWMO_Doodad_Instance::Render3D()
{
	//if (!m_QualitySettings.draw_wmo_doodads)
	//{
	//	return;
	//}

	//CM2_Base_Instance::Render3D();
}
