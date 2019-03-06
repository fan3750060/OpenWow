#pragma once

#include "UIBaseNode.h"

class CUIWindowNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUIWindowNode();
	virtual ~CUIWindowNode();

	// Parent & childs functional
	virtual void AddChild(std::shared_ptr<CUIBaseNode> pNode);
	virtual void RemoveChild(std::shared_ptr<CUIBaseNode> pNode);
	
	// CUIBaseNode
	virtual bool Accept(IVisitor& visitor);

	// Input events
	virtual bool OnKeyPressed(KeyEventArgs& e) override;
	virtual void OnKeyReleased(KeyEventArgs& e) override;
	virtual void OnMouseMoved(MouseMotionEventArgs& e) override;
	virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
	virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;
	virtual bool OnMouseWheel(MouseWheelEventArgs& e) override;

private:
	typedef std::vector<std::shared_ptr<CUIBaseNode>> NodeList;
	typedef std::multimap<std::string, std::shared_ptr<CUIBaseNode>> NodeNameMap;

	NodeList				    m_Children;
	NodeNameMap				    m_ChildrenByName;
};