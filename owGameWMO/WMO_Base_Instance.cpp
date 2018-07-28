#include "stdafx.h"

// General
#include "WMO_Base_Instance.h"

CWMO_Base_Instance::CWMO_Base_Instance(SceneNode* _parent, WMO* _wmoObject) :
	SceneNode(_parent),
	m_Object(_wmoObject)
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
	m_Object->CreateInsances(this);

	m_InvWorld = glm::inverse(getAbsTrans());
	if (m_Object->m_PortalController != nullptr)
	{
		for (auto& v : m_Object->m_PortalVertices)
		{
			m_ConvertedVerts.push_back(getAbsTrans() * vec4(v, 0));
		}
	}
}

void CWMO_Base_Instance::EmptyTransformAndBounds()
{
	// Matrix
	CalculateMatrix();

	// Bounds
	BoundingBox bbox;
	bbox.setMin(vec3(-100000.0f, -100000.0f, -100000.0f));
	bbox.setMax(vec3(100000.0f, 100000.0f, 100000.0f));
	bbox.calculateCenter();
	setBounds(bbox);
}

void CWMO_Base_Instance::Update(double _time, double _dTime)
{

}

#define WMO_DISABLE_PORTALS

bool CWMO_Base_Instance::PreRender3D()
{
	if (!checkDistance2D(m_QualitySettings.ADT_WMO_Distance))
	{
		return false;
	}

	if (!checkFrustum())
	{
		return false;
	}

#ifndef WMO_DISABLE_PORTALS
	if (m_Object->m_PortalController != nullptr)
	{
		m_Object->m_PortalController->Update(this, getInvWorld() * (_Render->getCamera()->Position));
	}
#endif

	return true;
}

void CWMO_Base_Instance::Render3D()
{
	if (!m_QualitySettings.draw_map_wmo)
	{
		return;
	}

	//_Render->DrawBoundingBox(m_Bounds);

	
	m_Object->Render(this);
}