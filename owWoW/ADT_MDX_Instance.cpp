#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

ADT_MDX_Instance::ADT_MDX_Instance(MDX* _mdxObject, ADT_MDDF _placementInfo) : 
    m_Object(_mdxObject)
{
    assert1(_mdxObject);
	m_UniqueId = _placementInfo.uniqueId;

	// Translate
	m_Translate = _placementInfo.position;

	// Rotate
	vec3 rotate = _placementInfo.rotation.toRad();
	rotate.x = -rotate.x;
	rotate.y = rotate.y - Math::PiHalf;
	m_Rotate = vec3(rotate.z, rotate.y, rotate.x);

	// Scale
	m_Scale = vec3(static_cast<float>(_placementInfo.scale) / 1024.0f);

	CalculateMatrix();
	_Bindings->RegisterRenderable3DObject(this, 21);
}

ADT_MDX_Instance::~ADT_MDX_Instance()
{
	_Bindings->UnregisterRenderable3DObject(this);
}

void ADT_MDX_Instance::PreRender3D(double t, double dt)
{
	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		SetVisible(false);
		return;
	}
	m_AlreadyDraw.insert(m_UniqueId);
	SetVisible(true);
}

void ADT_MDX_Instance::Render3D()
{
	_Pipeline->Clear();
	{
		_Pipeline->SetWorld(m_RelTransform);
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