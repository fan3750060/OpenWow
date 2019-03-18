#include "stdafx.h"

// General
#include "UISlateNodeParameter.h"

// Additional
#include "Application.h"
#include "UISlateEditor.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2   cParameterBackgroundSize  = vec2(240.0f, 32.0f);
	const vec4   cParameterBackgroundColor = vec4(0.43f, 0.43f, 0.43f, 1.0f);
}


CUISlateNodeParameter::CUISlateNodeParameter(std::weak_ptr<CUISlateEditor> Editor)
    : m_IsLinePointOpened(false)
    , m_Editor(Editor)
{

}

CUISlateNodeParameter::~CUISlateNodeParameter()
{
}



//
// CUISlateNodeParameter
//
void CUISlateNodeParameter::Initialize()
{
    m_Background = std::make_shared<CUITextureNode>(cParameterBackgroundSize);
    m_Background->SetParentInternal(weak_from_this());

    std::shared_ptr<Texture> backgroundTexture = _RenderDevice->CreateTexture2D("Textures\\Slate\\slate_parameter_32.png");
    m_Background->SetTexture(backgroundTexture);


}

void CUISlateNodeParameter::CreateDefault()
{
    m_LinePoint = std::make_shared<CUISlateConnectionPoint>(m_Editor);
    m_LinePoint->SetParentInternal(weak_from_this());
    m_LinePoint->Initialize();

    vec2 translate = vec2(0.0f, 0.0f);
    translate.y += m_Background->GetSize().y / 2.0f;
    translate -= (m_LinePoint->GetSize() / 2.0f);
    translate += glm::vec2(2.0f, 0.0f);


    m_LinePoint->SetTranslate(translate);
}

void CUISlateNodeParameter::SetText(const std::string& Text)
{
    //_ASSERT(m_Text != nullptr);

    //m_Text->SetText(Text);
}



//
// IUISlateConnectionable
//
vec2 CUISlateNodeParameter::GetConnectPoint() const
{
    cvec2 size = GetSize();
    return vec2(0.0f, size.y / 2.0f);
}

BoundingRect CUISlateNodeParameter::GetConnectRectangle() const
{
    return m_LinePoint->GetBoundsAbs();
}

IUISlateConnectionable::LineDefaultDirection CUISlateNodeParameter::GetConnectDirection() const
{
    return IUISlateConnectionable::LineDefaultDirection::DirectionLeft;
}



//
// CUIBaseNode
//
glm::vec2 CUISlateNodeParameter::GetSize() const
{
    return m_Background->GetSize();
}

std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNodeParameter::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    //if (m_Text)
    //    childs.push_back(m_Text);

    if (m_LinePoint)
        childs.push_back(m_LinePoint);

    return childs;
}



//
// Input events
//
bool CUISlateNodeParameter::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    return false;
}

void CUISlateNodeParameter::OnMouseButtonReleased(MouseButtonEventArgs & e)
{

}
