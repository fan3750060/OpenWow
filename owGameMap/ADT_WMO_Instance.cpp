#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

ADT_WMO_Instance::ADT_WMO_Instance(std::shared_ptr<WMO> _wmoObject, ADT_MODF& _placementInfo) :
	CWMO_Base_Instance(_wmoObject)
{
	m_UniqueId = _placementInfo.uniqueId;
	uint16 doodadSetIndex = _placementInfo.doodadSetIndex;
	m_DoodadSetInfo = _wmoObject->m_DoodadsSetInfos[doodadSetIndex];

	// Scene node params
	{
		// Translate
		setTranslate(_placementInfo.position);
		// Rotate
		vec3 rotate = glm::radians(_placementInfo.rotation);
		rotate.x = -rotate.x;
		rotate.y = rotate.y - glm::half_pi<float>();
		setRotate(vec3(rotate.z, rotate.y, rotate.x));
		//
		UpdateLocalTransform();
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

bool ADT_WMO_Instance::Accept(IVisitor& visitor)
{
	//if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	//{
	//	return false;
	//}

	// SceneNode
	if (CWMO_Base_Instance::Accept(visitor))
	{
		//m_AlreadyDraw.insert(m_UniqueId);
		return true;
	}

	return false;
}

//

void ADT_WMO_Instance::reset()
{
	m_AlreadyDraw.clear();
}
std::set<uint32> ADT_WMO_Instance::m_AlreadyDraw;