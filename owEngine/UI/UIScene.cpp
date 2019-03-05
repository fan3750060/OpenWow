#include "stdafx.h"


// General
#include "UIScene.h"

CUIScene::CUIScene()
{
	m_pRootNode = std::make_shared<CUIBaseNode>();
}

CUIScene::~CUIScene()
{
}

std::shared_ptr<CUIBaseNode> CUIScene::GetRootNode() const
{
	return m_pRootNode;
}

void CUIScene::Render(RenderUIEventArgs& renderEventArgs)
{
	if (m_pRootNode)
		m_pRootNode->Render(renderEventArgs);
}


//
// Input events
//
void CUIScene::OnKeyPressed(KeyEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnKeyPressed(e);
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

void CUIScene::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonPressed(e);
}

void CUIScene::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonReleased(e);
}

void CUIScene::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseWheel(e);
}
