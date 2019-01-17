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

void UISceneBase::Accept(IVisitor& visitor)
{
	if (m_pRootNode)
		m_pRootNode->Accept(visitor);
}

void UISceneBase::OnKeyPressed(KeyEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnKeyPressed(e);
}

void UISceneBase::OnKeyReleased(KeyEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnKeyReleased(e);
}

void UISceneBase::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseMoved(e);
}

void UISceneBase::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonPressed(e);
}

void UISceneBase::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonReleased(e);
}

void UISceneBase::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseWheel(e);
}
