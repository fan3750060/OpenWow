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

	// Text
	const char*  cParameterText = "Parameter name";
	const uint32 cParameterTextHeight = 16;
	const vec2   cParameterTextOffset = vec2(10.0f, 5.0f);
	const vec4   cParameterTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
    SetSize(cParameterBackgroundSize);

    m_Background = std::make_shared<CUIColorNode>(cParameterBackgroundSize);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(cParameterBackgroundColor);
}

void CUISlateNodeParameter::CreateDefault()
{
    m_LinePoint = std::make_shared<CUISlateConnectionPoint>(m_Editor);
    m_LinePoint->SetParentInternal(weak_from_this());
    m_LinePoint->Initialize();

    vec2 translate = vec2(0.0f, 0.0f);
    translate.y += m_Background->GetSize().y / 2.0f;
    translate -= (m_LinePoint->GetSize() / 2.0f);

    m_LinePoint->SetTranslate(translate);

    // 
	m_Text = std::make_shared<CUITextNode>();
	m_Text->SetParentInternal(weak_from_this());
	m_Text->SetText(cParameterText);
	m_Text->SetTranslate(cParameterTextOffset);
	m_Text->SetTextColor(cParameterTextColor);
	m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cParameterTextHeight));
}

void CUISlateNodeParameter::SetText(const std::string& Text)
{
    _ASSERT(m_Text != nullptr);

    m_Text->SetText(Text);
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
std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNodeParameter::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    if (m_Text)
        childs.push_back(m_Text);

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
