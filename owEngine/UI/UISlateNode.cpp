#include "stdafx.h"

// General
#include "UISlateNode.h"

// Additional
#include "UISlateEditor.h"


namespace
{
    // Background
    const vec2  cDefaultBackgroundSize = vec2(240.0f, 40.0f);
    const vec4  cDefaultBackgroundColor = vec4(0.33f, 0.33f, 0.33f, 1.0f);

    // Text
    const char* cDefaultText = "Parameter name";
    const uint32 cDefaultTextHeight = 16;
    const vec2  cDefaultTextOffset = vec2(10.0f, 5.0f);
    const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUISlateNode::CUISlateNode(std::weak_ptr<CUISlateEditor> Editor)
    : m_Editor(Editor)
{
    SetSize(cDefaultBackgroundSize);
}

CUISlateNode::~CUISlateNode()
{
}



//
// CUISlateNode
//
void CUISlateNode::CreateDefault()
{
    std::shared_ptr<CUISlateNodeHeader> header = std::make_shared<CUISlateNodeHeader>(m_Editor);
    header->CreateDefault();
    SetHeader(header);

    std::shared_ptr<CUISlateNodeParameter> param0 = std::make_shared<CUISlateNodeParameter>(m_Editor);
    param0->CreateDefault();
    param0->SetText("Parameter 0");
    AddParameter(param0);

    std::shared_ptr<CUISlateNodeParameter> param1 = std::make_shared<CUISlateNodeParameter>(m_Editor);
    param1->CreateDefault();
    param1->SetText("Parameter 1");
    AddParameter(param1);

    std::shared_ptr<CUISlateNodeParameter> param2 = std::make_shared<CUISlateNodeParameter>(m_Editor);
    param2->CreateDefault();
    param2->SetText("Parameter 2");
    AddParameter(param2);

    std::shared_ptr<CUISlateNodeFooter> footer = std::make_shared<CUISlateNodeFooter>(m_Editor);
    footer->CreateDefault();
    SetFooter(footer);
}

void CUISlateNode::SetHeader(std::shared_ptr<CUISlateNodeHeader> Header)
{
    _ASSERT(Header != nullptr);

    Header->SetParentInternal(weak_from_this());

    m_Header = Header;
}

void CUISlateNode::AddParameter(std::shared_ptr<CUISlateNodeParameter> Parameter)
{
    _ASSERT(Parameter != nullptr);

    // Calculate yTranslate
    float yTranslate = 0.0f;
    if (m_Header)
        yTranslate += m_Header->GetSize().y;
    for (auto it : m_Parameters)
        yTranslate += it.second->GetSize().y;
    Parameter->SetTranslate(vec2(0.0f, yTranslate));


    Parameter->SetParentInternal(weak_from_this());

    std::string parameterName = Parameter->GetName();
    if (!parameterName.empty())
    {
        m_Parameters.insert(SlateParameterNameMap::value_type(parameterName, Parameter));
    }

}

void CUISlateNode::RemoveParameter(std::shared_ptr<CUISlateNodeParameter> Parameter)
{
    _ASSERT(Parameter != nullptr);

    Parameter->SetParentInternal(std::weak_ptr<CUIBaseNode>());

    SlateParameterNameMap::iterator iter = m_Parameters.find(Parameter->GetName());
    if (iter != m_Parameters.end())
    {
        m_Parameters.erase(iter);
    }
}

void CUISlateNode::SetFooter(std::shared_ptr<CUISlateNodeFooter> Footer)
{
    _ASSERT(Footer != nullptr);

    // Calculate yTranslate
    float yTranslate = 0.0f;
    if (m_Header)
        yTranslate += m_Header->GetSize().y;
    for (auto it : m_Parameters)
        yTranslate += it.second->GetSize().y;

    Footer->SetTranslate(vec2(0.0f, yTranslate));

    Footer->SetParentInternal(weak_from_this());

    m_Footer = Footer;
}



glm::vec2 CUISlateNode::GetSize() const
{
    vec2 size = vec2(cDefaultBackgroundSize.x, 0.0f);

    for (auto ch : GetChilds())
    {
        size.y += ch->GetSize().y;
    }

    return size;
}


//
// CUIBaseNode
//
std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNode::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Header)
        childs.push_back(m_Header);

    for (auto it : m_Parameters)
        childs.push_back(it.second);

    if (m_Footer)
        childs.push_back(m_Footer);

    return childs;
}



//
// Input events
void CUISlateNode::OnMouseMoved(MouseMotionEventArgs & e)
{
    for (auto ch : GetChilds())
    {
        _ASSERT(ch);
        ch->OnMouseMoved(e);
    }
}

bool CUISlateNode::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    bool result = false;
    for (auto ch : GetChilds())
    {
        _ASSERT(ch);
        if (ch->IsPointInBoundsAbs(e.GetPoint()))
        {
            if (ch->OnMouseButtonPressed(e))
            {
                result = true;
                break;
            }
        }
    }

    return result;
}

void CUISlateNode::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    for (auto ch : GetChilds())
    {
        _ASSERT(ch);
        ch->OnMouseButtonReleased(e);
    }
}
