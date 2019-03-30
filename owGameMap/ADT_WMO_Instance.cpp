#include "stdafx.h"

// General
#include "ADT_WMO_Instance.h"

// Additional
#include "WMO\\WMO_Pass.h"

ADT_WMO_Instance::ADT_WMO_Instance(std::string _wmoName, ADT_MODF& _placementInfo) :
	CWMO_Base_Instance(_wmoName)
{
	m_UniqueId = _placementInfo.uniqueId;
	uint16 doodadSetIndex = _placementInfo.doodadSetIndex;
	//m_DoodadSetInfo = _wmoObject->m_DoodadsSetInfos[doodadSetIndex];

	// Scene node params
	{
		// Translate
		SetTranslate(_placementInfo.position);

		// Rotate
		vec3 rotate = glm::radians(_placementInfo.rotation);
		rotate.x = -rotate.x;
		rotate.y = rotate.y - glm::half_pi<float>();
		SetRotation(vec3(rotate.z, rotate.y, rotate.x));

		// Bounds
		BoundingBox bbox(_placementInfo.boundingBox.min, _placementInfo.boundingBox.max);
		SetBounds(bbox);
	}
}

ADT_WMO_Instance::~ADT_WMO_Instance()
{
	//Log::Info("ADT_WMO Deleted");
}

bool ADT_WMO_Instance::Accept(IVisitor& visitor)
{
	const WMO_Pass* passAsWMOPass = dynamic_cast<const WMO_Pass*>(&visitor);
	if (passAsWMOPass)
	{
		if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
		{
			return false;
		}
	}

	// SceneNode3D
	if (CWMO_Base_Instance::Accept(visitor))
	{
		if (passAsWMOPass)
		{
			m_AlreadyDraw.insert(m_UniqueId);
		}
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