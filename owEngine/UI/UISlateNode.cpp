#include "stdafx.h"

// General
#include "UISlateNode.h"

// Additional
#include "Application.h"

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


CUISlateNode::CUISlateNode()
{
}

CUISlateNode::~CUISlateNode()
{
}



//
// CUISlateNode
//
void CUISlateNode::CreateDefault()
{
    std::shared_ptr<CUISlateNodeHeader> header = std::make_shared<CUISlateNodeHeader>();
    header->CreateDefault();
    SetHeader(header);

    std::shared_ptr<CUISlateNodeParameter> param0 = std::make_shared<CUISlateNodeParameter>();
    param0->CreateDefault();
    AddParameter(param0);

    std::shared_ptr<CUISlateNodeParameter> param1 = std::make_shared<CUISlateNodeParameter>();
    param1->CreateDefault();
    AddParameter(param1);

    std::shared_ptr<CUISlateNodeFooter> footer = std::make_shared<CUISlateNodeFooter>();
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

    Parameter->SetParentInternal(std::weak_ptr<CUIWindowNode>());

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



//
// CUIBaseNode
//
void CUISlateNode::SetParent(std::weak_ptr<CUIBaseNode> parent)
{
    fail1("Don't call this method! Use 'AddChild' instead!");
}

bool CUISlateNode::Accept(IVisitor & visitor)
{
    bool visitResult = base::Accept(visitor);
    if (!visitResult)
        return false;

    if (m_Header != nullptr)
    {
        m_Header->Accept(visitor);
    }

    for (auto it : m_Parameters)
    {
        if (it.second != nullptr)
        {
            it.second->Accept(visitor);
        }
    }

    if (m_Footer != nullptr)
    {
        m_Footer->Accept(visitor);
    }

    return visitResult;
}

bool CUISlateNode::AcceptMesh(IVisitor & visitor)
{
    return false;
}
