#include "stdafx.h"


// General
#include "UIScene.h"

CUIScene::CUIScene()
{
	m_pRootNode = std::make_shared<CUIWindowNode>();
}

CUIScene::~CUIScene()
{
}

std::shared_ptr<CUIWindowNode> CUIScene::GetRootNode() const
{
	return m_pRootNode;
}

void CUIScene::Accept(IVisitor & visitor)
{
	if (m_pRootNode)
		m_pRootNode->Accept(visitor);
}


//
// Input events
//
bool CUIScene::OnKeyPressed(KeyEventArgs & e)
{
	if (m_pRootNode)
		return m_pRootNode->OnKeyPressed(e);

	return false;
}

void CUIScene::OnKeyReleased(KeyEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnKeyReleased(e);
}

void CUIScene::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseMoved(e);
}

bool CUIScene::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		return m_pRootNode->OnMouseButtonPressed(e);

	return false;
}

void CUIScene::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonReleased(e);
}

bool CUIScene::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_pRootNode)
		return m_pRootNode->OnMouseWheel(e);

	return false;
}
