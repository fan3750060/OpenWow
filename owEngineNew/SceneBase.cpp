#include "stdafx.h"

// Include
#include "SceneNode.h"

// General
#include "SceneBase.h"

SceneBase::SceneBase()
{
	m_pRootNode = make_shared<SceneNode>();
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
	std::shared_ptr<SceneNode> node = make_shared<SceneNode>();

	node->SetParent(parent != nullptr ? parent : GetRootNode());

	return node;
}

void SceneBase::ImportSceneNode(std::shared_ptr<SceneNode> parent, std::shared_ptr<Mesh> _mesh)
{
	std::shared_ptr<SceneNode> node = make_shared<SceneNode>();
	node->AddMesh(_mesh);

	node->SetParent(parent);
}

void SceneBase::Render(RenderEventArgs& args)
{
	if (m_pRootNode)
	{
		m_pRootNode->Render(args);
	}
}

void SceneBase::Accept(IVisitor& visitor)
{
	visitor.Visit(*this);

	if (m_pRootNode)
	{
		m_pRootNode->Accept(visitor);
	}
}