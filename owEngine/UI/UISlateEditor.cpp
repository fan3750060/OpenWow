#include "stdafx.h"

// General
#include "UISlateEditor.h"

CUISlateEditor::CUISlateEditor()
{
}

CUISlateEditor::~CUISlateEditor()
{
    m_Children.clear();
}

void CUISlateEditor::CreateDefault(vec2 Position, vec2 Size)
{
    SetTranslate(Position);
    SetSize(Size);

    m_Background = std::make_shared<CUIColorNode>(Size);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(vec4(1.0f, 0.5f, 0.0f, 0.8f));
}

//
// Parent & childs functional
//
void CUISlateEditor::AddChild(std::shared_ptr<CUISlateNode> Node)
{
    _ASSERT(Node != nullptr);

    NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), Node);
    if (iter == m_Children.end())
    {
        Node->SetParentInternal(weak_from_this());

        m_Children.push_back(Node);
        if (!Node->GetName().empty())
        {
            m_ChildrenByName.insert(NodeNameMap::value_type(Node->GetName(), Node));
        }
    }
}

void CUISlateEditor::RemoveChild(std::shared_ptr<CUISlateNode> Node)
{
    _ASSERT(Node != nullptr);

    NodeList::iterator iter = std::find(m_Children.begin(), m_Children.end(), Node);
    if (iter != m_Children.end())
    {
        Node->SetParent(std::weak_ptr<CUISlateEditor>());

        m_Children.erase(iter);

        // Also remove it from the name map.
        NodeNameMap::iterator iter2 = m_ChildrenByName.find(Node->GetName());
        if (iter2 != m_ChildrenByName.end())
        {
            m_ChildrenByName.erase(iter2);
        }
    }
}

//
// CUIBaseNode
//
bool CUISlateEditor::Accept(IVisitor& visitor)
{
    bool visitResult = base::Accept(visitor);
    if (!visitResult)
        return false;

    _ASSERT(m_Background != nullptr);
    m_Background->Accept(visitor);

    for (auto child : m_Children)
    {
        child->Accept(visitor);
    }

    return true;
}

//
// Input events
//
bool CUISlateEditor::OnKeyPressed(KeyEventArgs & e)
{
    // Childs
    bool result = false;
    for (auto it : m_Children)
    {
        if (it->OnKeyPressed(e))
            result = true;
    }
    return result;
}

void CUISlateEditor::OnKeyReleased(KeyEventArgs & e)
{
    for (auto it : m_Children)
    {
        it->OnKeyPressed(e);
    }
}

void CUISlateEditor::OnMouseMoved(MouseMotionEventArgs& e)
{
    for (auto it : m_Children)
    {
        MouseMotionEventArgs eInner(e);
        e.X -= static_cast<int>(GetTranslation().x);
        e.Y -= static_cast<int>(GetTranslation().y);

        it->OnMouseMoved(eInner);

        // Synteric events impl
        if (it->IsPointInBounds(eInner.GetPoint()))
        {
            if (!it->IsMouseOnNode())
            {
                it->OnMouseEntered();
                it->DoMouseEntered();
            }
        }
        else
        {
            if (it->IsMouseOnNode())
            {
                it->OnMouseLeaved();
                it->DoMouseLeaved();
            }
        }
    }
}

bool CUISlateEditor::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    bool result = false;
    for (auto it : m_Children)
    {
        MouseButtonEventArgs eInner(e);
        e.X -= static_cast<int>(GetTranslation().x);
        e.Y -= static_cast<int>(GetTranslation().y);

        if (it->IsPointInBounds(eInner.GetPoint()))
            if (it->OnMouseButtonPressed(eInner))
                result = true;
    }
    return result;
}

void CUISlateEditor::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    for (auto it : m_Children)
    {
        MouseButtonEventArgs eInner(e);
        e.X -= static_cast<int>(GetTranslation().x);
        e.Y -= static_cast<int>(GetTranslation().y);

        it->OnMouseButtonReleased(eInner);
    }
}

bool CUISlateEditor::OnMouseWheel(MouseWheelEventArgs & e)
{
    bool result = false;
    for (auto it : m_Children)
    {
        MouseWheelEventArgs eInner(e);
        e.X -= static_cast<int>(GetTranslation().x);
        e.Y -= static_cast<int>(GetTranslation().y);

        if (it->IsPointInBounds(eInner.GetPoint()))
            if (it->OnMouseWheel(eInner))
                result = true;
    }
    return result;
}