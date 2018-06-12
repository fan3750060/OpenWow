#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(SceneNode* _parent, WMO* _wmoObject, ADT_MODF& _placementInfo) :
	SceneNode(_parent),
    m_Object(_wmoObject)
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
		m_Bounds.Min = _placementInfo.boundingBox.min; // Don't use from WMO model!!!
		m_Bounds.Max = _placementInfo.boundingBox.max;
		m_Bounds.calculateCenter();
	}

	_wmoObject->CreateInsances(this);

	setDrawOrder(21);
	setSelectable();
	_Bindings->RegisterUpdatableObject(this);
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void ADT_WMO_Instance::Update(double _time, double _dTime)
{
	m_Object->Update(_time, _dTime);
}

void ADT_WMO_Instance::PreRender3D()
{
	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		setVisible(false);
		return;
	}
	m_AlreadyDraw.insert(m_UniqueId);

	setVisible(!_CameraFrustum->_frustum.cullBox(m_Bounds));
}

void ADT_WMO_Instance::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);

	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(getAbsTrans());
		m_Object->Render(m_DoodadSetIndex);
		PERF_INC(PERF_MAP_MODELS_WMOs);
	}
}

//

void ADT_WMO_Instance::reset()
{
	m_AlreadyDraw.clear();
}

set<uint32> ADT_WMO_Instance::m_AlreadyDraw;
