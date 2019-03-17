#include "stdafx.h"

// General
#include "UISlateNodeFooter.h"

// Additional
#include "Application.h"
#include "UISlateEditor.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2   cFooterBackgroundSize  = vec2(240.0f, 32.0f);
	const vec4   cFooterBackgroundColor = vec4(0.23f, 0.23f, 0.23f, 1.0f);

    // Text
    const char*  cHeaderTextFontPath = "Fonts\\JustBreatheBd.otf";
    const char*  cFooterText = "Result: ";
    const uint32 cFooterTextHeight = 16;
    const vec2   cFooterTextOffset = vec2(5.0f, 5.0f);
    const vec4   cFooterTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUISlateNodeFooter::CUISlateNodeFooter(std::weak_ptr<CUISlateEditor> Editor)
    : m_Editor(Editor)
{

}

CUISlateNodeFooter::~CUISlateNodeFooter()
{
}



//
// CUISlateNodeFooter
//
void CUISlateNodeFooter::Initialize()
{
    SetSize(cFooterBackgroundSize);

    m_Background = std::make_shared<CUIColorNode>(cFooterBackgroundSize);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(cFooterBackgroundColor);
}

void CUISlateNodeFooter::CreateDefault()
{
    m_LinePoint = std::make_shared<CUISlateConnectionPoint>(m_Editor);
    m_LinePoint->SetParentInternal(weak_from_this());
    m_LinePoint->Initialize();

    // Calculate translate
    vec2 translate = cFooterBackgroundSize;
    translate.y /= 2.0f;
    translate -= (m_LinePoint->GetSize() / 2.0f);

    m_LinePoint->SetTranslate(translate);


    m_Text = std::make_shared<CUITextNode>();
    m_Text->SetParentInternal(weak_from_this());
    m_Text->SetText(cFooterText);
    m_Text->SetTranslate(cFooterTextOffset);
    m_Text->SetTextColor(cFooterTextColor);
    m_Text->SetFont(GetManager<IFontsManager>()->Add(cHeaderTextFontPath, cFooterTextHeight));
}



//
// IUISlateConnectionable
//
vec2 CUISlateNodeFooter::GetConnectPoint() const
{
    cvec2 size = GetSize();
    return vec2(size.x, (size.y / 2.0f));
}

BoundingRect CUISlateNodeFooter::GetConnectRectangle() const
{
    return m_LinePoint->GetBoundsAbs();
}

IUISlateConnectionable::LineDefaultDirection CUISlateNodeFooter::GetConnectDirection() const
{
    return IUISlateConnectionable::LineDefaultDirection::DirectionRight;
}



//
// CUIBaseNode
//
std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNodeFooter::GetChilds() const
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
bool CUISlateNodeFooter::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    return false;
}

void CUISlateNodeFooter::OnMouseButtonReleased(MouseButtonEventArgs & e)
{

}
