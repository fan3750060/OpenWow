#include "stdafx.h"

// General
#include "SceneNode.h"

SceneNode::SceneNode() : 
	m_Parent(nullptr),
	m_Scale(vec3(1.0f, 1.0f, 1.0f))
{}

SceneNode::SceneNode(SceneNode* _parent) : 
	m_Parent(_parent),
	m_Scale(vec3(1.0f, 1.0f, 1.0f))
{
	m_Parent->m_Childs.push_back(this);
}

void SceneNode::CalculateMatrix()
{
	// Build relative matrix
	m_RelTransform.translate(m_Translate);
	m_RelTransform.rotate(m_Rotate);
	m_RelTransform.scale(m_Scale);

	if (m_Parent != nullptr)
	{
		m_AbsTransform = (m_Parent->getAbsTrans()) * m_RelTransform;
	}
	else
	{
		m_AbsTransform = m_RelTransform;
	}
}