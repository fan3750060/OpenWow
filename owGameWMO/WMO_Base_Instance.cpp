#include "stdafx.h"

// General
#include "WMO_Base_Instance.h"

CWMO_Base_Instance::CWMO_Base_Instance(SceneNode* _parent, WMO* _wmoObject) :
	SceneNode(_parent),
	m_Object(_wmoObject),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	assert1(m_Object);

	setDebugColor(vec4(0.0f, 0.0f, 1.0f, 0.9f));
	setSelectable();

	//_Bindings->RegisterUpdatableObject(this);
}

CWMO_Base_Instance::~CWMO_Base_Instance()
{
	//_Bindings->UnregisterUpdatableObject(this);
}

void CWMO_Base_Instance::InitTransform()
{
	m_InstanceController = new CWMO_InstanceController(m_Object, getAbsTrans());
	m_Object->CreateInsances(this);
}

void CWMO_Base_Instance::InitDefault()
{
	// Matrix
	CalculateMatrix();
	// Bounds
	m_Bounds.setMin(vec3(-100000.0f, -100000.0f, -100000.0f));
	m_Bounds.setMax(vec3(100000.0f, 100000.0f, 100000.0f));
	m_Bounds.calculateCenter();
}

void CWMO_Base_Instance::Update(double _time, double _dTime)
{

}

bool CWMO_Base_Instance::PreRender3D()
{
	float distToCamera2D = (_Render->getCamera()->Position.toX0Z() - getBounds().getCenter().toX0Z()).length() - getBounds().getRadius();
	if (distToCamera2D > m_QualitySettings.ADT_WMO_Distance)
	{
		return false;
	}

	if (_Render->getCamera()->_frustum.cullBox(m_Bounds))
	{
		return false;
	}

	return true;
}

void CWMO_Base_Instance::Render3D()
{
	if (!m_QualitySettings.draw_map_wmo)
	{
		return;
	}

	//_Render->DrawBoundingBox(m_Bounds);

	m_Object->PreRender(m_InstanceController);
	m_Object->Render(m_InstanceController, m_DoodadSetInfo);
}