#include "stdafx.h"

// Include
#include "SceneNodeUI.h"

// General
#include "SceneUI.h"

SceneUI::SceneUI()
{
	m_pRootNode = std::make_shared<SceneNodeUI>();
}

SceneUI::~SceneUI()
{
}

std::shared_ptr<SceneNodeUI> SceneUI::GetRootNode() const
{
	return m_pRootNode;
}

void SceneUI::Render(RenderUIEventArgs& renderEventArgs)
{
	if (m_pRootNode)
		m_pRootNode->Render(renderEventArgs);
}

void SceneUI::OnKeyPressed(KeyEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnKeyPressed(e);
}

void SceneUI::OnKeyReleased(KeyEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnKeyReleased(e);
}

void SceneUI::OnMouseMoved(MouseMotionEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseMoved(e);
}

void SceneUI::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonPressed(e);
}

void SceneUI::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseButtonReleased(e);
}

void SceneUI::OnMouseWheel(MouseWheelEventArgs & e)
{
	if (m_pRootNode)
		m_pRootNode->OnMouseWheel(e);
}
