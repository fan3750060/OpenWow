#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

ADT_MDX_Instance::ADT_MDX_Instance(SceneNode* _parent, M2* _mdxObject, const ADT_MDXDef& _placementInfo) :
	CM2_Base_Instance(_parent, _mdxObject)
{
	m_UniqueId = _placementInfo.uniqueId;

	// Scene node params
	{
		// Translate
		setTranslate(_placementInfo.position, false);
		// Rotate
		vec3 rotate = _placementInfo.rotation.toRad();
		rotate.x = -rotate.x;
		rotate.y = rotate.y - Math::PiHalf;
		setRotate(vec3(rotate.z, rotate.y, rotate.x));
		// Scale
		setScale(static_cast<float>(_placementInfo.scale) / 1024.0f);

		CalculateMatrix();
	}

	InitLocal();
	
	setDrawOrder(21);
	setDebugColor(vec4(1.0f, 0.0f, 1.0f, 1.0f));
	setSelectable();
}

ADT_MDX_Instance::~ADT_MDX_Instance()
{
	//Log::Info("ADT_MDX Deleted");
}

bool ADT_MDX_Instance::PreRender3D()
{
	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		return false;
	}

	// Check distance to camera
	if (!checkDistance2D(m_QualitySettings.ADT_MDX_Distance))
	{
		return false;
	}

	if (!CM2_Base_Instance::PreRender3D())
	{
		return false;
	}

	m_AlreadyDraw.insert(m_UniqueId);

	return true;
}

void ADT_MDX_Instance::Render3D()
{
	if (!m_QualitySettings.draw_map_m2)
	{
		return;
	}

	CM2_Base_Instance::Render3D();

	PERF_INC(PERF_MAP_MODELS_MDXs);
}

//
set<uint32> ADT_MDX_Instance::m_AlreadyDraw;
void ADT_MDX_Instance::reset()
{
	m_AlreadyDraw.clear();
}
