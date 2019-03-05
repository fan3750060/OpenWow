#pragma once

#include "UIBaseNode.h"

class CUIScene : public Object
{
public:
	CUIScene();
	virtual ~CUIScene();

	std::shared_ptr<CUIBaseNode> GetRootNode() const;

	void Render(RenderUIEventArgs& renderEventArgs);

	// Input events
	void OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);
	void OnMouseMoved(MouseMotionEventArgs& e);
	void OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	void OnMouseWheel(MouseWheelEventArgs& e);

private:
	std::shared_ptr<CUIBaseNode> m_pRootNode;
};
