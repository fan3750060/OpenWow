#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UIColor.h"
#include "UIText.h"
#include "UITexture.h"

// Slate
#include "UISlateEditorInterfaces.h"
#include "UISlateConnectionPoint.h"

// FORWARD BEGIN
class CUISlateEditor;
class CUISlateNode;
// FORWARD END

class CUISlateNodeFooter : 
    public CUIBaseNode, 
    public IUISlateConnectionable
{
    typedef CUIBaseNode base;
public:
    CUISlateNodeFooter(std::weak_ptr<CUISlateEditor> Editor);
    virtual ~CUISlateNodeFooter();

    // CUISlateNodeHeader
    void Initialize();
    void CreateDefault();

    // IUISlateConnectionable
    vec2 GetConnectPoint() const;
    BoundingRect GetConnectRectangle() const;
    LineDefaultDirection GetConnectDirection() const;

    // CUIBaseNode
    std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

    // Input events
    bool OnMouseButtonPressed(MouseButtonEventArgs& e) override final;
    void OnMouseButtonReleased(MouseButtonEventArgs& e) override final;

private:
    std::shared_ptr<CUIColorNode>               m_Background;
    std::shared_ptr<CUITextNode>                m_Text;
    std::shared_ptr<CUISlateConnectionPoint>    m_LinePoint;

    // Weak refs
    std::weak_ptr<CUISlateEditor>               m_Editor;
};