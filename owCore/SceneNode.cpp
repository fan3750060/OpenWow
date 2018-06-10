#include "stdafx.h"

// General
#include "SceneNode.h"

SceneNode::SceneNode() : 
	m_Parent(nullptr),
	m_Rotate(vec3(0.0f, 0.0f, 0.0f)),
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_IsVisible(false),
	m_DrawOrder(0)
{
	//CalculateMatrix();
}

SceneNode::SceneNode(SceneNode* _parent) : 
	m_Parent(_parent),
	m_Rotate(vec3(0.0f, 0.0f, 0.0f)),
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_IsVisible(false),
	m_DrawOrder(0)
{
	//CalculateMatrix();

	if (m_Parent != nullptr)
	{
		m_Parent->addChild(this);
	}
}

SceneNode::~SceneNode()
{
	for (auto it : m_Childs)
	{
		it->setParent(nullptr);
		//fail1();
	}

	if (m_Parent != nullptr)
	{
		m_Parent->removeChild(this);
	}
}

bool SceneNode::Load()
{
	return true;
}

bool SceneNode::Delete()
{
	return true;
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
}