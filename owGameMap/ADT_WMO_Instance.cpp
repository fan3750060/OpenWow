#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(std::weak_ptr<SceneNode> _parent, SmartWMOPtr _wmoObject, ADT_MODF& _placementInfo) :
	CWMO_Base_Instance(_parent, _wmoObject)
{
	m_UniqueId = _placementInfo.uniqueId;
	uint16 doodadSetIndex = _placementInfo.doodadSetIndex;
	m_DoodadSetInfo = _wmoObject->m_DoodadsSetInfos[doodadSetIndex];

	// Scene node params
	{
		// Translate
		setTranslate(_placementInfo.position, false);
		// Rotate
		vec3 rotate = glm::radians(_placementInfo.rotation);
		rotate.x = -rotate.x;
		rotate.y = rotate.y - glm::half_pi<float>();
		setRotate(vec3(rotate.z, rotate.y, rotate.x), false);
		// Matrix
		CalculateLocalTransform();
		// Bounds
		BoundingBox bbox(_placementInfo.boundingBox.min, _placementInfo.boundingBox.max);
		setBounds(bbox);
	}
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
	//Log::Info("ADT_WMO Deleted");
}

void ADT_WMO_Instance::Load()
{
	InitTransform();
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
}

//

void ADT_WMO_Instance::reset()
{
	m_AlreadyDraw.clear();
}
std::set<uint32> ADT_WMO_Instance::m_AlreadyDraw;