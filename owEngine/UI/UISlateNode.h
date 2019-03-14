#pragma once

// Common UI
#include "UIBaseNode.h"

// Slate
#include "UISlateEditorInterfaces.h"
#include "UISlateNodeHeader.h"
#include "UISlateNodeParameter.h"
#include "UISlateNodeFooter.h"

// FORWARD BEGIN
class CUISlateEditor;
// FORWARD END

class CUISlateNode : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
	CUISlateNode(std::weak_ptr<CUISlateEditor> Editor);
	virtual ~CUISlateNode();

	// CUISlateNode
	void CreateDefault();

    // Content
    void SetHeader(std::shared_ptr<CUISlateNodeHeader> Header);
    void AddParameter(std::shared_ptr<CUISlateNodeParameter> Parameter);
    void RemoveParameter(std::shared_ptr<CUISlateNodeParameter> Parameter);
    void SetFooter(std::shared_ptr<CUISlateNodeFooter> Footer);


    //
	// CUIBaseNode
    //

    // Size & bounds functional
    glm::vec2 GetSize() const override;

    virtual std::vector<std::shared_ptr<CUIBaseNode>> GetChilds() const override;

    // Input events
    virtual void OnMouseMoved(MouseMotionEventArgs& e) override;
    virtual bool OnMouseButtonPressed(MouseButtonEventArgs& e) override;
    virtual void OnMouseButtonReleased(MouseButtonEventArgs& e) override;

private:
    typedef std::unordered_multimap<std::string, std::shared_ptr<CUISlateNodeParameter>> SlateParameterNameMap;

	std::shared_ptr<CUISlateNodeHeader>     m_Header;
    SlateParameterNameMap                   m_Parameters;
    std::shared_ptr<CUISlateNodeFooter>     m_Footer;

    // Weak refs
    std::weak_ptr<CUISlateEditor>           m_Editor;
};