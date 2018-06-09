#include "stdafx.h"

// General
#include "SceneNode.h"

SceneNode::SceneNode() : 
	m_Parent(nullptr),
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_IsVisible(false),
	m_DrawOrder(0)
{
	CalculateMatrix();

	//_Bindings->RegisterRenderable3DObject(this);
	_Bindings->RegisterUpdatableObject(this);
}

SceneNode::SceneNode(SceneNode* _parent) : 
	m_Parent(_parent),
	m_Scale(vec3(1.0f, 1.0f, 1.0f)),
	m_DrawOrder(0)
{
	CalculateMatrix();

	if (m_Parent != nullptr)
	{
		m_Parent->m_Childs.push_back(this);
	}

	//_Bindings->RegisterRenderable3DObject(this);
	_Bindings->RegisterUpdatableObject(this);
}

SceneNode::~SceneNode()
{
	if (m_Parent != nullptr)
	{
		m_Parent->getChilds().erase(std::remove(m_Parent->getChilds().begin(), m_Parent->getChilds().end(), this), m_Parent->getChilds().end());
	}

	_Bindings->UnregisterUpdatableObject(this);
	//_Bindings->UnregisterRenderable3DObject(this);
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