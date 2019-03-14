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
	const vec2  cDefaultBackgroundSize  = vec2(240.0f, 32.0f);
	const vec4  cDefaultBackgroundColor = vec4(0.43f, 0.43f, 0.43f, 1.0f);

    // Line point 
    const float cDefaultIconSize = 12.0f;
    const vec4  cLinePointOpenedColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    const vec4  cLinePointClosedColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);

	// Text
	const char* cDefaultText = "Parameter name";
	const uint32 cDefaultTextHeight = 16;
	const vec2  cDefaultTextOffset = vec2(10.0f, 5.0f);
	const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
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
void CUISlateNodeParameter::CreateDefault()
{
    SetSize(cDefaultBackgroundSize);

	m_Background = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
	m_Background->SetParentInternal(weak_from_this());
	m_Background->SetColor(cDefaultBackgroundColor);

    m_LinePoint = std::make_shared<CUITextureNode>(vec2(cDefaultIconSize, cDefaultIconSize));
    m_LinePoint->SetParentInternal(weak_from_this());
    m_LinePoint->SetTexture(_RenderDevice->CreateTexture2D("Textures\\slate_round_filled.png"));

    vec2 translate = vec2(0.0f, 0.0f);
    translate.y += m_Background->GetSize().y / 2.0f;
    translate -= (m_LinePoint->GetSize() / 2.0f);

    m_LinePoint->SetTranslate(translate);

    // 
	m_Text = std::make_shared<CUITextNode>();
	m_Text->SetParentInternal(weak_from_this());
	m_Text->SetText(cDefaultText);
	m_Text->SetTranslate(cDefaultTextOffset);
	m_Text->SetTextColor(cDefaultTextColor);
	m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}

void CUISlateNodeParameter::SetText(std::string Text)
{
    m_Text->SetText(Text);
}

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
    if (GetConnectRectangle().isPointInside(e.GetPoint()))
    {
        std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
        _ASSERT(editor);

        editor->BeginMakeConnection(std::dynamic_pointer_cast<IUISlateConnectionable>(shared_from_this()));

        return true;
    }

    return false;
}

void CUISlateNodeParameter::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
    if (GetConnectRectangle().isPointInside(e.GetPoint()))
    {
        std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
        _ASSERT(editor);

        editor->FinishMakeConnection(std::dynamic_pointer_cast<IUISlateConnectionable>(shared_from_this()));
    }
}
