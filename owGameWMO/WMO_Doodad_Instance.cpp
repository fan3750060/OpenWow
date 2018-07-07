#include "stdafx.h"

// General
#include "WMO_Doodad_Instance.h"

CWMO_Doodad_Instance::CWMO_Doodad_Instance(SceneNode* _parent, M2* _mdxObject, uint32 _index, const SWMO_Doodad_PlacementInfo& _placement) :
	CM2_Base_Instance(_parent, _mdxObject),
	m_Index(_index),
	m_PortalVis(true)
{
	if (_mdxObject->getFilename().find("LD_LIGHTSHAFT") != -1)
	{
		Log::Green("MODEL [%s] contains color [%f, %f, %f, %f]", _mdxObject->getFilename().c_str(), _placement.getColor().x, _placement.getColor().y, _placement.getColor().z, _placement.getColor().w);
	}

	//m_DoodadColor = _placement.getColor();

	//m_Object->setDoodadColor(_placement.getColor());
	// Scene node params
	{
		// Convert
		m_Translate = _placement.position.toXZmY();
		m_RotateQuat = Quaternion(-_placement.orientation.z, _placement.orientation.x, _placement.orientation.y, _placement.orientation.w);
		m_Scale = vec3(_placement.scale, -_placement.scale, -_placement.scale);
		//
		CalculateMatrix(true);
		//
		m_Bounds = m_Object->getBounds();
		m_Bounds.transform(getAbsTrans());
	}

	InitLocal();

	setDrawOrder(22);
	setDebugColor(vec4(0.0f, 1.0f, 1.0f, 1.0f));
	setSelectable();
}

CWMO_Doodad_Instance::~CWMO_Doodad_Instance()
{
	//Log::Info("ADT_MDX Deleted");
}

bool CWMO_Doodad_Instance::PreRender3D()
{
	if (!m_PortalVis)
	{
		return false;
	}

	// Check distance to camera
	float distToCamera = (_Render->getCamera()->Position - getBounds().getCenter()).length() - getBounds().getRadius();
	if (distToCamera > m_QualitySettings.WMO_MODD_Distance)
	{
		return false;
	}

	// Frustrum culling
	if (!CM2_Base_Instance::PreRender3D())
	{
		return false;
	}

	return true;
}

void CWMO_Doodad_Instance::Render3D()
{
	if (!m_QualitySettings.draw_wmo_doodads)
	{
		return;
	}

	CM2_Base_Instance::Render3D();

	PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
}


