#include "stdafx.h"

// General
#include "UISlateNode.h"

// Additional
#include "UISlateEditor.h"


// TEMP DELETE ME
#include "UISlateNodeParameter_List.h"

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
  
}

CUISlateNode::~CUISlateNode()
{
}



//
// CUISlateNode
//

void CUISlateNode::Initialize()
{
    std::shared_ptr<CUISlateNodeHeader> header = std::make_shared<CUISlateNodeHeader>(m_Editor);
    header->Initialize("Operation name", "Textures\\Operations\\LogMessage.png");
    SetHeader(header);
}

void CUISlateNode::CreateDefault()
{
    std::shared_ptr<CUISlateNodeParameter> param0 = std::make_shared<CUISlateNodeParameter>(m_Editor);
    param0->Initialize();
    param0->CreateDefault();
    param0->SetText("Parameter 0");
    AddParameter(param0);

    std::shared_ptr<CUISlateNodeParameter> param1 = std::make_shared<CUISlateNodeParameter>(m_Editor);
    param1->Initialize();
    param1->CreateDefault();
    param1->SetText("Parameter 1");
    AddParameter(param1);


    std::shared_ptr<CUISlateNodeParameter_List> paramList = std::make_shared<CUISlateNodeParameter_List>(m_Editor);
    paramList->Initialize(std::vector<std::string>());
    paramList->SetText("Parameter List");
    AddParameter(paramList);

    std::shared_ptr<CUISlateNodeParameter> param2 = std::make_shared<CUISlateNodeParameter>(m_Editor);
    param2->Initialize();
    param2->CreateDefault();
    param2->SetText("Parameter 2");
    AddParameter(param2);

    std::shared_ptr<CUISlateNodeFooter> footer = std::make_shared<CUISlateNodeFooter>(m_Editor);
    footer->Initialize();
    footer->CreateDefault();
    SetFooter(footer);
}

void CUISlateNode::SetHeader(std::shared_ptr<CUISlateNodeHeader> Header)
{
    _ASSERT(Header != nullptr);

    Header->SetParentInternal(weak_from_this());
    m_Header = Header;

    CalculateChildsTranslate();
}

std::shared_ptr<CUISlateNodeHeader> CUISlateNode::GetHeader() const
{
    return m_Header;
}

void CUISlateNode::AddParameter(std::shared_ptr<CUISlateNodeParameter> Parameter)
{
    _ASSERT(Parameter != nullptr);

    Parameter->SetParentInternal(weak_from_this());

    std::string parameterName = Parameter->GetName();
    if (!parameterName.empty())
    {
        m_Parameters.insert(SlateParameterNameMap::value_type(parameterName, Parameter));
    }

    CalculateChildsTranslate();
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

    Footer->SetParentInternal(weak_from_this());
    m_Footer = Footer;

    CalculateChildsTranslate();
}





//
// CUIBaseNode
//
glm::vec2 CUISlateNode::GetSize() const
{
    vec2 size = vec2(cDefaultBackgroundSize.x, 0.0f);

    for (auto ch : GetChilds())
    {
        size.y += ch->GetSize().y;
    }

    return size;
}

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



//
// Protected
//
void CUISlateNode::CalculateChildsTranslate()
{
    // Header
    // don't touch it position

    // Parameters
    glm::vec2 parametersStartPosition = m_Header->GetTranslation() + glm::vec2(0.0f, m_Header->GetSize().y);
    glm::vec2 parameterCurrentPosition = parametersStartPosition;

    for (auto parameterIter : m_Parameters)
    {
        std::shared_ptr<CUISlateNodeParameter> parameter = parameterIter.second;
        _ASSERT(parameter != nullptr);

        parameter->SetTranslate(parameterCurrentPosition);

        parameterCurrentPosition += glm::vec2(0.0f, parameter->GetSize().y);
    }

    // Footer
    if (m_Footer)
        m_Footer->SetTranslate(parameterCurrentPosition);
}
