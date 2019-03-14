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
	const vec2  cDefaultBackgroundSize  = vec2(240.0f, 32.0f);
	const vec4  cDefaultBackgroundColor = vec4(0.23f, 0.23f, 0.23f, 1.0f);

    // Line point 
    const float cDefaultIconSize = 12.0f;

    // Text
    const char* cDefaultText = "Result: ";
    const uint32 cDefaultTextHeight = 16;
    const vec2  cDefaultTextOffset = vec2(5.0f, 5.0f);
    const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
void CUISlateNodeFooter::CreateDefault()
{
    SetSize(cDefaultBackgroundSize);

	m_Background = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
	m_Background->SetParentInternal(weak_from_this());
	m_Background->SetColor(cDefaultBackgroundColor);


    m_LinePoint = std::make_shared<CUITextureNode>(vec2(cDefaultIconSize, cDefaultIconSize));
    m_LinePoint->SetParentInternal(weak_from_this());
    m_LinePoint->SetTexture(_RenderDevice->CreateTexture2D("Textures\\slate_round_filled.png"));

    // Calculate translate
    vec2 translate = cDefaultBackgroundSize;
    translate.y /= 2.0f;
    translate -= (m_LinePoint->GetSize() / 2.0f);

    m_LinePoint->SetTranslate(translate);


    m_Text = std::make_shared<CUITextNode>();
    m_Text->SetParentInternal(weak_from_this());
    m_Text->SetText(cDefaultText);
    m_Text->SetTranslate(cDefaultTextOffset);
    m_Text->SetTextColor(cDefaultTextColor);
    m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}

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
    if (GetConnectRectangle().isPointInside(e.GetPoint()))
    {
        std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
        _ASSERT(editor);

        editor->BeginMakeConnection(std::dynamic_pointer_cast<IUISlateConnectionable>(shared_from_this()));

        return true;
    }

    return false;
}

void CUISlateNodeFooter::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    if (GetConnectRectangle().isPointInside(e.GetPoint()))
    {
        std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
        _ASSERT(editor);

        editor->FinishMakeConnection(std::dynamic_pointer_cast<IUISlateConnectionable>(shared_from_this()));
    }
}
