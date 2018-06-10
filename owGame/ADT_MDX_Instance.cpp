#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

// Additional
#include "WorldController.h"

ADT_MDX_Instance::ADT_MDX_Instance(SceneNode* _parent, M2* _mdxObject, const ADT_MDDF& _placementInfo) :
	SceneNode(_parent),
    m_Object(_mdxObject)
{
    assert1(_mdxObject);
	m_UniqueId = _placementInfo.uniqueId;

	// Scene node params
	{
		// Translate
		m_Translate = _placementInfo.position;
		// Rotate
		vec3 rotate = _placementInfo.rotation.toRad();
		rotate.x = -rotate.x;
		rotate.y = rotate.y - Math::PiHalf;
		m_Rotate = vec3(rotate.z, rotate.y, rotate.x);
		// Scale
		m_Scale = vec3(static_cast<float>(_placementInfo.scale) / 1024.0f);
		//
		CalculateMatrix();
		//
		m_Bounds = m_Object->GetBounds();
		m_Bounds.transform(getAbsTrans());
	}
	
	SetDrawOrder(20);
	_Bindings->RegisterUpdatableObject(this);
}

ADT_MDX_Instance::~ADT_MDX_Instance()
{
	_Bindings->UnregisterUpdatableObject(this);
}

void ADT_MDX_Instance::Update(double _time, double _dTime)
{
	m_Object->Update(_time, _dTime);
}

void ADT_MDX_Instance::PreRender3D()
{
	/*if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		SetVisible(false);
		return;
	}
	m_AlreadyDraw.insert(m_UniqueId);*/
	SetVisible(!_CameraFrustum->_frustum.cullBox(m_Bounds));
}

void ADT_MDX_Instance::Render3D()
{
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(getAbsTrans());
		m_Object->Render();
		PERF_INC(PERF_MAP_MODELS_MDXs);
	}
	_Pipeline->Clear();
}

//
set<uint32> ADT_MDX_Instance::m_AlreadyDraw;
void ADT_MDX_Instance::reset()
{
	m_AlreadyDraw.clear();
}
