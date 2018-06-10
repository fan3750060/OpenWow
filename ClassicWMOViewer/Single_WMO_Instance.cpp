#include "stdafx.h"

// General
#include "Single_WMO_Instance.h"

Single_WMO_Instance::Single_WMO_Instance(SceneNode* _parent, WMO* _wmoObject) :
	SceneNode(_parent),
	m_Object(_wmoObject)
{
	assert1(m_Object);

	// Scene node params
	{
		// Translate
		//m_Translate = vec3(100, 0, 100);
		// Rotate
		//m_Rotate = vec3(Math::TwoPi, Math::TwoPi, Math::PiHalf);
		//
		CalculateMatrix();
		//
		m_Bounds = m_Object->m_Bounds;
		m_Bounds.transform(getAbsTrans());
	}

	_wmoObject->CreateInsances(this);

	//SetDrawOrder(20);
	//_Bindings->RegisterUpdatableObject(this);
}

Single_WMO_Instance::~Single_WMO_Instance()
{
	//_Bindings->UnregisterUpdatableObject(this);
}

void Single_WMO_Instance::Update(double _time, double _dTime)
{
	m_Object->Update(_time, _dTime);
}

void Single_WMO_Instance::PreRender3D()
{
	/*if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
	SetVisible(false);
	return;
	}
	m_AlreadyDraw.insert(m_UniqueId);*/
	SetVisible(!_CameraFrustum->_frustum.cullBox(m_Bounds));
}

void Single_WMO_Instance::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);

	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(getAbsTrans());
		m_Object->Render(0);
		PERF_INC(PERF_MAP_MODELS_WMOs);
	}
}