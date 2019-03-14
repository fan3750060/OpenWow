#pragma once

// Common UI
#include "UIBaseNode.h"
#include "UIBeizerLine.h"

// Slate
#include "UISlateEditorInterfaces.h"

// FORWARD BEGIN
class CUISlateNode;
class CUISlateEditor;
// FORWARD END

class CUISlateConnection : public CUIBaseNode
{
	typedef CUIBaseNode base;
public:
    CUISlateConnection(std::weak_ptr<CUISlateEditor> Editor);
	virtual ~CUISlateConnection();

	// CUISlateConnection
    void Create(std::shared_ptr<IUISlateConnectionable> Begin, std::shared_ptr<IUISlateConnectionable> End);
    void OnNodesMoved(UIBaseNodeMovedEventArgs& args);


	// CUIBaseNode
	virtual bool Accept(IVisitor& visitor) override;
	virtual bool AcceptMesh(IVisitor& visitor) override;

protected:
    void RefreshLine();

private:
	std::shared_ptr<CUIBeizerLineNode>            m_Line;

    std::shared_ptr<IUISlateConnectionable> m_Begin;
    std::shared_ptr<IUISlateConnectionable> m_End;

    // Weak refs
    std::weak_ptr<CUISlateEditor>           m_Editor;
};