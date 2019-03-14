#include "stdafx.h"

// General
#include "UISlateEditor.h"

CUISlateEditor::CUISlateEditor()
    : m_CurrentConnectionable(nullptr)
{
}

CUISlateEditor::~CUISlateEditor()
{
    m_Nodes.clear();
}

void CUISlateEditor::CreateDefault(vec2 Position, vec2 Size)
{
    SetTranslate(Position);
    SetSize(Size);

    m_Background = std::make_shared<CUIColorNode>(Size);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(vec4(0.0f, 0.3f, 0.9f, 0.8f));
}

//
// Parent & childs functional
//
void CUISlateEditor::AddNode(std::shared_ptr<CUISlateNode> Node)
{
    _ASSERT(Node != nullptr);

    NodeList::iterator iter = std::find(m_Nodes.begin(), m_Nodes.end(), Node);
    if (iter == m_Nodes.end())
    {
        Node->SetParentInternal(weak_from_this());

        m_Nodes.push_back(Node);
        if (!Node->GetName().empty())
        {
            m_NodesByName.insert(NodeNameMap::value_type(Node->GetName(), Node));
        }
    }
}

void CUISlateEditor::RemoveNode(std::shared_ptr<CUISlateNode> Node)
{
    _ASSERT(Node != nullptr);

    NodeList::iterator iter = std::find(m_Nodes.begin(), m_Nodes.end(), Node);
    if (iter != m_Nodes.end())
    {
        Node->SetParentInternal(std::weak_ptr<CUISlateEditor>());

        m_Nodes.erase(iter);

        NodeNameMap::iterator iter2 = m_NodesByName.find(Node->GetName());
        if (iter2 != m_NodesByName.end())
        {
            m_NodesByName.erase(iter2);
        }
    }
}



//
// Connect / disconnect
//
void CUISlateEditor::BeginMakeConnection(std::shared_ptr<IUISlateConnectionable> Initiator)
{
    m_CurrentConnectionable = Initiator;

    // Vars
    std::shared_ptr<CUIBaseNode> InitiatorAsBaseNode = std::dynamic_pointer_cast<CUIBaseNode>(Initiator);
    vec2 connectPoint = Initiator->GetConnectPoint() + InitiatorAsBaseNode->GetTranslationAbs() - GetTranslation();

    // Current line
    m_CurrentLine = std::make_shared<CUIBeizerLineNode>();
    m_CurrentLine->SetParentInternal(weak_from_this());
    m_CurrentLine->SetPosStart(connectPoint);
    m_CurrentLine->SetPosEnd(connectPoint);
    m_CurrentLine->SetColor(vec4(1.0f, 0.5f, 0.0f, 1.0f));
}

void CUISlateEditor::FinishMakeConnection(std::shared_ptr<IUISlateConnectionable> Target)
{
    if (m_CurrentConnectionable == nullptr)
    {
        return;
    }

    if (m_CurrentConnectionable == Target)
    {
        return;
    }

    // Current line
    m_CurrentLine.reset();

    MakeConnection(m_CurrentConnectionable, Target);
}

void CUISlateEditor::BeginMoveNode(std::shared_ptr<CUISlateNode> Node, glm::vec2 Point)
{
    m_MovingNodes_Current = Node;
    m_MovingNodes_Point = Point;
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

    for (auto child : m_Nodes)
    {
        child->Accept(visitor);
    }

    for (auto conn : m_Connections)
    {
        conn->Accept(visitor);
    }

    if (m_CurrentLine)
        m_CurrentLine->Accept(visitor);

    return true;
}

//
// Input events
//
bool CUISlateEditor::OnKeyPressed(KeyEventArgs & e)
{
    // Childs
    bool result = false;
    for (auto it : m_Nodes)
    {
        if (it->OnKeyPressed(e))
            result = true;
    }
    return result;
}

void CUISlateEditor::OnKeyReleased(KeyEventArgs & e)
{
    for (auto it : m_Nodes)
    {
        it->OnKeyPressed(e);
    }
}

void CUISlateEditor::OnMouseMoved(MouseMotionEventArgs& e)
{
    if (m_CurrentLine) 
    {
        m_CurrentLine->SetPosEnd(e.GetPoint() - GetTranslationAbs());
    }

    if (m_MovingNodes_Current)
    {
        m_MovingNodes_Current->SetTranslate(e.GetPoint() - m_MovingNodes_Current->GetParent()->GetTranslationAbs() - m_MovingNodes_Point);
    }

    for (auto it : m_Nodes)
    {
        it->OnMouseMoved(e);

        // Synteric events impl
        if (it->IsPointInBoundsAbs(e.GetPoint()))
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
    for (auto it : m_Nodes)
    {
        if (it->IsPointInBoundsAbs(e.GetPoint()))
            if (it->OnMouseButtonPressed(e))
                result = true;
    }
    return result;
}

void CUISlateEditor::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    for (auto it : m_Nodes)
    {
        it->OnMouseButtonReleased(e);
    }

    m_CurrentConnectionable = nullptr;
    m_CurrentLine = nullptr;

    m_MovingNodes_Current = nullptr;
}

bool CUISlateEditor::OnMouseWheel(MouseWheelEventArgs & e)
{
    bool result = false;
    for (auto it : m_Nodes)
    {
        if (it->IsPointInBoundsAbs(e.GetPoint()))
            if (it->OnMouseWheel(e))
                result = true;
    }
    return result;
}

void CUISlateEditor::MakeConnection(std::shared_ptr<IUISlateConnectionable> Initiator, std::shared_ptr<IUISlateConnectionable> Target)
{
    _ASSERT(Initiator != nullptr);
    _ASSERT(Target != nullptr);

    std::shared_ptr<CUISlateConnection> newConnection = std::make_shared<CUISlateConnection>(std::dynamic_pointer_cast<CUISlateEditor>(shared_from_this()));
    newConnection->Create(Initiator, Target);
    m_Connections.push_back(newConnection);
}
