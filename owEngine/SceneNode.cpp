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
	float distToCamera2D = (_Render->getCamera()->Position.toX0Z() - getBounds().getCenter().toX0Z()).length() - getBounds().getRadius();
	return distToCamera2D < _distance;
}

bool SceneNode::checkDistance(float _distance) const
{
	// Check distance to camera
	float distToCamera = (_Render->getCamera()->Position - getBounds().getCenter()).length() - getBounds().getRadius();
	return distToCamera < _distance;
}

void SceneNode::CalculateMatrix(bool _isRotationQuat)
{
	m_RelTransform = mat4();

	m_RelTransform.translate(m_Translate);
	if (_isRotationQuat)
	{
		m_RelTransform *= m_RotateQuat;
	}
	else
	{
		m_RelTransform.rotate(m_Rotate);
	}
	m_RelTransform.scale(m_Scale);

	//--

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}

	/*if (m_Childs.size() > 0)
	{
		for (auto& ch : m_Childs)
		{
			ch->CalculateMatrix();
		}
	}*/
}