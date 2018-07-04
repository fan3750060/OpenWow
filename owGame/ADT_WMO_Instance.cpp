#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(SceneNode* _parent, WMO* _wmoObject, ADT_MODF& _placementInfo) :
	CWMO_Base_Instance(_parent, _wmoObject)
{
	m_UniqueId = _placementInfo.uniqueId;
	uint16 doodadSetIndex = _placementInfo.doodadSetIndex;
	m_DoodadSetInfo = _wmoObject->m_DoodadsSetInfos[doodadSetIndex];

	// Scene node params
	{
		// Translate
		m_Translate = _placementInfo.position;
		// Rotate
		vec3 rotate = _placementInfo.rotation.toRad();
		rotate.x = -rotate.x;
		rotate.y = rotate.y - Math::PiHalf;
		m_Rotate = vec3(rotate.z, rotate.y, rotate.x);
		//m_Rotate = vec3(0.0f, Random::GenerateRange(0.0f, Math::Pi) , 0.0f);
		// Matrix
		CalculateMatrix();
		// Bounds
		m_Bounds.setMin(_placementInfo.boundingBox.min); // Don't use from WMO model!!!
		m_Bounds.setMax(_placementInfo.boundingBox.max);
		m_Bounds.calculateCenter();
	}

	InitTransform();

	setDrawOrder(21);
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
	//Log::Info("ADT_WMO Deleted");
}

bool ADT_WMO_Instance::PreRender3D()
{
	if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	{
		return false;
	}

	if (!CWMO_Base_Instance::PreRender3D())
	{
		return false;
	}

	m_AlreadyDraw.insert(m_UniqueId);

	return true;
}

void ADT_WMO_Instance::Render3D()
{
	CWMO_Base_Instance::Render3D();
	PERF_INC(PERF_MAP_MODELS_WMOs);
}

//

void ADT_WMO_Instance::reset()
{
	m_AlreadyDraw.clear();
}
set<uint32> ADT_WMO_Instance::m_AlreadyDraw;
