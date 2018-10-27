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

std::shared_ptr<SceneNode> SceneBase::CreateSceneNode(std::shared_ptr<SceneNode> parent)
{
	std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();

	node->SetParent(parent != nullptr ? parent : GetRootNode());

	return node;
}

void SceneBase::ImportSceneNode(std::shared_ptr<SceneNode> parent, std::shared_ptr<IMesh> _mesh)
{
	std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>();
	node->AddMesh(_mesh);

	node->SetParent(parent);
}

void SceneBase::Accept(IVisitor& visitor)
{
	if (m_pRootNode)
	{
		m_pRootNode->Accept(visitor);
	}
}