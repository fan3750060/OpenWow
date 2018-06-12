#include "stdafx.h"

// General
#include "WMO_MODD_Instance.h"

WMO_MODD_Instance::WMO_MODD_Instance(SceneNode* _parent, M2* _mdxObject, const WMO_MODD_PlacementInfo& _placement) :
	SceneNode(_parent),
	m_Object(_mdxObject)
{
	// Scene node params
	{
		// Convert
		m_Translate = _placement.position.toXZmY();
		m_RotateQuat = Quaternion(-_placement.orientation.z, _placement.orientation.x, _placement.orientation.y, _placement.orientation.w);;
		m_Scale = vec3(_placement.scale, -_placement.scale, -_placement.scale);
		//
		CalculateMatrix(true);
		//
		m_Bounds = m_Object->getBounds();
		m_Bounds.transform(getAbsTrans());
	}

	setDrawOrder(23);
	setSelectable();
	_Bindings->RegisterUpdatableObject(this);
}

WMO_MODD_Instance::~WMO_MODD_Instance()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void WMO_MODD_Instance::Update(double _time, double _dTime)
{
	m_Object->Update(_time, _dTime);
}

void WMO_MODD_Instance::PreRender3D()
{
	setVisible(!_CameraFrustum->_frustum.cullBox(m_Bounds));
}

void WMO_MODD_Instance::Render3D()
{
	//_Render->DrawBoundingBox(m_Bounds);

	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(getAbsTrans());
		m_Object->Render();
		PERF_INC(PERF_MAP_MODELS_WMOs_DOODADS);
	}
}


