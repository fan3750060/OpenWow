#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

// Additional
#include "WorldController.h"

ADT_MDX_Instance::ADT_MDX_Instance(SceneNode* _parent, M2* _mdxObject, ADT_MDDF _placementInfo) :
	SceneNode(_parent),
    m_Object(_mdxObject)
{
    assert1(_mdxObject);
	m_UniqueId = _placementInfo.uniqueId;

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
	}
	
	SetDrawOrder(21);
	Load();
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
	SetVisible(true);
}

void ADT_MDX_Instance::Render3D()
{
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_AbsTransform);
		m_Object->Render();
		PERF_INC(PERF_MAP_MODELS_MDXs);
	}
}

//

void ADT_MDX_Instance::reset()
{
	m_AlreadyDraw.clear();
}

set<uint32> ADT_MDX_Instance::m_AlreadyDraw;