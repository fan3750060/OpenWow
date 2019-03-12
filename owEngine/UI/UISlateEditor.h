#pragma once

// Base UI
#include "UIWindow.h"

// Slate nodes
#include "UISlateNode.h"

// Common UI
#include "UIColor.h"

class CUISlateEditor : public CUIBaseNode
{
    typedef CUIBaseNode base;
public:
    CUISlateEditor();
    virtual ~CUISlateEditor();

    // CUISlateEditor
    void CreateDefault(vec2 Position, vec2 Size);

    // Parent & childs functional
    void AddChild(std::shared_ptr<CUISlateNode> Node);
    void RemoveChild(std::shared_ptr<CUISlateNode> Node);

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
    typedef std::vector<std::shared_ptr<CUISlateNode>> NodeList;
    typedef std::multimap<std::string, std::shared_ptr<CUISlateNode>> NodeNameMap;

    std::shared_ptr<CUIColorNode>       m_Background;

    NodeList				            m_Children;
    NodeNameMap				            m_ChildrenByName;
};