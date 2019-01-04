#include "stdafx.h"

// General
#include "ADT_MDX_Instance.h"

ADT_MDX_Instance::ADT_MDX_Instance(std::shared_ptr<M2> _mdxObject, const ADT_MDXDef& _placementInfo) :
	CM2_Base_Instance(_mdxObject)
{
	m_UniqueId = _placementInfo.uniqueId;

	// Scene node params
	{
		setTranslate(_placementInfo.position);
		vec3 rotate = glm::radians(_placementInfo.rotation);
		rotate.x = -rotate.x;
		rotate.y = rotate.y - glm::half_pi<float>();
		setRotate(vec3(rotate.z, rotate.y, rotate.x));
		setScale(vec3(static_cast<float>(_placementInfo.scale) / 1024.0f));
	}

	InitAnimator();
}

ADT_MDX_Instance::~ADT_MDX_Instance()
{
	//Log::Info("ADT_MDX Deleted");
}


//
// SceneNode
//
bool ADT_MDX_Instance::Accept(IVisitor & visitor)
{
	const BasePass& visitorAsBasePass = reinterpret_cast<BasePass&>(visitor);
	const Camera& camera = *(visitorAsBasePass.GetRenderEventArgs().Camera);
	
	//if (m_AlreadyDraw.find(m_UniqueId) != m_AlreadyDraw.end())
	//{
	//	return false;
	//}

	/*float distToCamera2D = (camera.GetTranslation() - getBounds().getCenter()).length() - getBounds().getRadius();
	if (distToCamera2D > m_QualitySettings.ADT_MCNK_Distance)
	{
		return;
	}*/

	// SceneNode
	if (CM2_Base_Instance::Accept(visitor))
	{
		//m_AlreadyDraw.insert(m_UniqueId);
		return true;
	}

	return false;
}

//
std::set<uint32> ADT_MDX_Instance::m_AlreadyDraw;
void ADT_MDX_Instance::reset()
{
	m_AlreadyDraw.clear();
}