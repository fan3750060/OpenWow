#include "stdafx.h"

// Include
#include "UINode.h"

// General
#include "UISceneBase.h"

UISceneBase::UISceneBase()
{
	m_pRootNode = std::make_shared<UINode>();
}

UISceneBase::~UISceneBase()
{
}

std::shared_ptr<UINode> UISceneBase::GetRootNode() const
{
	return m_pRootNode;
}

std::shared_ptr<UINode> UISceneBase::CreateSceneNode(std::shared_ptr<UINode> parent)
{
	std::shared_ptr<UINode> node = std::make_shared<UINode>();

	node->SetParent(parent != nullptr ? parent : GetRootNode());

	return node;
}

void UISceneBase::ImportSceneNode(std::shared_ptr<UINode> parent, std::shared_ptr<IMesh> _mesh)
{
	std::shared_ptr<UINode> node = std::make_shared<UINode>();
	node->AddMesh(_mesh);

	node->SetParent(parent);
}

void UISceneBase::Accept(IVisitor& visitor)
{
	if (m_pRootNode)
	{
		m_pRootNode->Accept(visitor);
	}
}