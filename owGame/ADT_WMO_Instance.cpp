#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(SceneNode* _parent, WMO* _wmoObject, ADT_MODF& _placementInfo) :
	SceneNode(_parent),
    m_Object(_wmoObject),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>()),
	m_DistancesSettings(GetSettingsGroup<CGroupDistances>())
{
    assert1(m_Object);
	m_UniqueId = _placementInfo.uniqueId;
	m_DoodadSetIndex = _placementInfo.doodadSetIndex;

	// Scene node params
	{
		// Translate
		m_Translate = _placementInfo.position;
		// Rotate
		vec3 rotate = _placementInfo.rotation.toRad();
		rotate.x = -rotate.x;
		rotate.y = rotate.y - Math::PiHalf;
		m_Rotate = vec3(rotate.z, rotate.y, rotate.x);
		//
		CalculateMatrix();
		//
		m_Bounds.setMin(_placementInfo.boundingBox.min); // Don't use from WMO model!!!
		m_Bounds.setMax(_placementInfo.boundingBox.max);
		m_Bounds.calculateCenter();
	}

	m_LocalPortalController = new CWMO_LocalPortalContainer(m_Object, getAbsTrans());

	m_Object->CreateInsances(this);

	setDrawOrder(21);
	setDebugColor(vec4(0.0f, 0.0f, 1.0f, 1.0f));
	setSelectable();
	//_Bindings->RegisterUpdatableObject(this);
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
	//_Bindings->UnregisterUpdatableObject(this);
}

void ADT_WMO_Instance::Update(double _time, double _dTime)
{

}

void ADT_WMO_Instance::PreRender3D()
{
	setVisible(false);

	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		return;
	}

	// Check distance to camera
	float distToCamera = (_Render->getCamera()->Position.toX0Z() - getBounds().getCenter().toX0Z()).length();
	if (distToCamera > m_DistancesSettings.ADT_WMO_Distance)
	{
		return;
	}

	// Frustrum culling
	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return;
	}

	m_AlreadyDraw.insert(m_UniqueId);
	setVisible(true);
}

void ADT_WMO_Instance::Render3D()
{
	if (!m_QualitySettings.draw_map_wmo)
	{
		return;
	}

	//_Render->DrawBoundingBox(m_Bounds);

	m_Object->Render(m_LocalPortalController, m_DoodadSetIndex);
	PERF_INC(PERF_MAP_MODELS_WMOs);
}

//

void ADT_WMO_Instance::reset()
{
	m_AlreadyDraw.clear();
}
set<uint32> ADT_WMO_Instance::m_AlreadyDraw;
