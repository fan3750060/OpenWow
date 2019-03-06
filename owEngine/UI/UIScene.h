#pragma once

#include "UIBaseNode.h"
#include "UIWindow.h"

class CUIScene : public Object
{
public:
	CUIScene();
	virtual ~CUIScene();

	std::shared_ptr<CUIWindowNode> GetRootNode() const;

	void Accept(IVisitor& visitor);

	// Input events
	bool OnKeyPressed(KeyEventArgs& e);
	void OnKeyReleased(KeyEventArgs& e);
	void OnMouseMoved(MouseMotionEventArgs& e);
	bool OnMouseButtonPressed(MouseButtonEventArgs& e);
	void OnMouseButtonReleased(MouseButtonEventArgs& e);
	bool OnMouseWheel(MouseWheelEventArgs& e);

private:
	std::shared_ptr<CUIWindowNode> m_pRootNode;
};
