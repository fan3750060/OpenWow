#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(WMO* _wmoObject, ADT_MODF& _placementInfo) : 
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
		// Bounds
		m_Bounds.Min = _wmoObject->m_Header.bounding_box.min.toXZY();
		m_Bounds.Max = _wmoObject->m_Header.bounding_box.max.toXZY();
		m_Bounds.calculateInternal();	
		//
		CalculateMatrix();
		//
		m_Bounds.transform(m_AbsTransform);
	}

	_Bindings->RegisterRenderable3DObject(this, 21);
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

void ADT_WMO_Instance::PreRender3D(double _time, double _dTime)
{
	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		SetVisible(false);
		return;
	}
	m_AlreadyDraw.insert(m_UniqueId);
	SetVisible(!_CameraFrustum->_frustum.cullBox(m_Bounds));
}

void ADT_WMO_Instance::Render3D()
{
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_AbsTransform);
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
