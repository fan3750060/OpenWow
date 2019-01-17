#include "stdafx.h"

// Include
#include "SceneNode.h"

// General
#include "SceneBase.h"

SceneBase::SceneBase()
{
	m_pRootNode = std::make_shared<SceneNode>();
}

SceneBase::~SceneBase()
{
}

std::shared_ptr<SceneNode> SceneBase::GetRootNode() const
{
	return m_pRootNode;
}

void SceneBase::Accept(IVisitor& visitor)
{
	if (m_pRootNode)
		m_pRootNode->Accept(visitor);
}

void SceneBase::OnUpdate(UpdateEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnUpdate(e);
}
