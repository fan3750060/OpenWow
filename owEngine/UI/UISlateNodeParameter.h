#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UIColor.h"
#include "UIText.h"
#include "UITexture.h"

// Slate
#include "UISlateEditorInterfaces.h"

// FORWARD BEGIN
class CUISlateEditor;
class CUISlateNode;
// FORWARD END

class CUISlateNodeParameter : 
    public CUIBaseNode,
    public IUISlateConnectionable
{
	typedef CUIBaseNode base;
public:
	CUISlateNodeParameter(std::weak_ptr<CUISlateEditor> Editor);
	virtual ~CUISlateNodeParameter();

	// CUISlateNodeParameter
	void CreateDefault();
    void SetText(std::string Text);

    // IUISlateConnectionable
    vec2 GetConnectPoint() const;
    BoundingRect GetConnectRectangle() const;
    LineDefaultDirection GetConnectDirection() const;

	// CUIBaseNode
    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

    // Input events
    bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

private:
	std::shared_ptr<CUIColorNode>       m_Background;
	std::shared_ptr<CUITextNode>        m_Text;
    std::shared_ptr<CUITextureNode>     m_LinePoint;

    bool                                m_IsLinePointOpened;

    // Weak refs
    std::weak_ptr<CUISlateEditor>       m_Editor;
};