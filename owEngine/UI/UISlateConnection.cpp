#include "stdafx.h"

// General
#include "UISlateConnection.h"

// Additional
#include "Application.h"
#include "UISlateEditor.h"
#include "UISlateNode.h"


CUISlateConnection::CUISlateConnection(std::weak_ptr<CUISlateEditor> Editor)
    : m_Editor(Editor)
{

}

CUISlateConnection::~CUISlateConnection()
{
}

void CUISlateConnection::Create(std::shared_ptr<IUISlateConnectionable> Begin, std::shared_ptr<IUISlateConnectionable> End)
{
    m_Begin = Begin;
    m_End = End;

    m_Line = std::make_shared<CUIBeizerLineNode>();
    m_Line->SetParentInternal(m_Editor);

    RefreshLine();

    std::shared_ptr<CUIBaseNode> BeginAsBaseNode = std::dynamic_pointer_cast<CUIBaseNode>(m_Begin);
    BeginAsBaseNode->Moved.connect(&CUISlateConnection::OnNodesMoved, this, std::placeholders::_1);

    std::shared_ptr<CUIBaseNode> EndAsBaseNode = std::dynamic_pointer_cast<CUIBaseNode>(m_End);
    EndAsBaseNode->Moved.connect(&CUISlateConnection::OnNodesMoved, this, std::placeholders::_1);
}

void CUISlateConnection::OnNodesMoved(UIBaseNodeMovedEventArgs & args)
{
    RefreshLine();
}



//
// CUIBaseNode
//

bool CUISlateConnection::Accept(IVisitor & visitor)
{
	bool visitResult = base::Accept(visitor);
	if (!visitResult)
		return false;

	if (m_Line != nullptr)
	{
        m_Line->Accept(visitor);
	}

	return visitResult;
}

bool CUISlateConnection::AcceptMesh(IVisitor & visitor)
{
	return false;
}

void CUISlateConnection::RefreshLine()
{
    std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
    _ASSERT(editor != nullptr);

    std::shared_ptr<CUIBaseNode> BeginAsBaseNode = std::dynamic_pointer_cast<CUIBaseNode>(m_Begin);
    vec2 BeginConnectPoint = m_Begin->GetConnectPoint() + BeginAsBaseNode->GetTranslationAbs() - editor->GetTranslation();

    std::shared_ptr<CUIBaseNode> EndAsBaseNode = std::dynamic_pointer_cast<CUIBaseNode>(m_End);
    vec2 EndConnectPoint = m_End->GetConnectPoint() + EndAsBaseNode->GetTranslationAbs() - editor->GetTranslation();

    // Current line
    m_Line->SetPosStart(BeginConnectPoint);
    m_Line->SetPosEnd(EndConnectPoint);
    m_Line->SetColor(vec4(1.0f, 1.0f, 1.0f, 1.0f));
}
