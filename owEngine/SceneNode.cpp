#include "stdafx.h"

// General
#include "SceneNode.h"

SceneNode::SceneNode(SceneNode* _parent) : 
	m_Parent(_parent),
	m_Rotate(vec3(0.0f, 0.0f, 0.0f)),
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_Selectable(false),
	m_IsVisible(false),
	m_IsOpaque(false),
	m_DrawOrder(0),
	m_DebugColor(vec4(0.5f, 0.5f, 0.5f, 0.5f)),
	m_QualitySettings(GetSettingsGroup<CGroupQuality>())
{
	if (m_Parent != nullptr)
	{
		m_Parent->addChild(this);
	}
}

SceneNode::~SceneNode()
{
	if (m_Parent != nullptr)
	{
		m_Parent->removeChild(this);
	}
}

bool SceneNode::checkFrustum() const
{
	return !_Render->getCamera()->_frustum.cullBox(getBounds());
}

bool SceneNode::checkDistance2D(float _distance) const
{
	// Check distance to camera
	float distToCamera2D = glm::length(Fix_X0Z(_Render->getCamera()->Position) - Fix_X0Z(getBounds().getCenter())) - getBounds().getRadius();
	return distToCamera2D < _distance;
}

bool SceneNode::checkDistance(float _distance) const
{
	// Check distance to camera
	float distToCamera = glm::length(_Render->getCamera()->Position - getBounds().getCenter()) - getBounds().getRadius();
	return distToCamera < _distance;
}

void SceneNode::CalculateLocalTransform(bool _isRotationQuat)
{
	m_RelTransform = mat4();

	m_RelTransform = glm::translate(m_RelTransform, m_Translate);
	if (_isRotationQuat)
	{
		m_RelTransform *= glm::toMat4(m_RotateQuat);
	}
	else
	{
		m_RelTransform = glm::rotate(m_RelTransform, m_Rotate.x, vec3(1, 0, 0));
		m_RelTransform = glm::rotate(m_RelTransform, m_Rotate.y, vec3(0, 1, 0));
		m_RelTransform = glm::rotate(m_RelTransform, m_Rotate.z, vec3(0, 0, 1));
	}
	m_RelTransform = glm::scale(m_RelTransform, m_Scale);

	//--

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->GetWorldTransfom()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}

	/*if (m_Childs.size() > 0)
	{
		for (auto& ch : m_Childs)
		{
			ch->CalculateLocalTransform();
		}
	}*/
}