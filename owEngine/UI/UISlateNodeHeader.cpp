#include "stdafx.h"

// General
#include "UISlateNodeHeader.h"

// Additional
#include "Application.h"
#include "UISlateEditor.h"
#include "UISlateNode.h"


namespace
{
	// Background
	const vec2  cDefaultBackgroundSize  = vec2(240.0f, 56.0f);
	const vec4  cDefaultBackgroundColor = vec4(0.33f, 0.33f, 0.33f, 1.0f);

	// Icon
	const vec2  cDefaultIconOffset = vec2(5.0f, 5.0f);
	const float cDefaultIconSize   = 32.0f;

	// Text
	const char* cDefaultText = "Some test action";
	const uint32 cDefaultTextHeight = 16;
	const vec2  cDefaultTextOffset = vec2(5.0f, 0.0f);
	const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUISlateNodeHeader::CUISlateNodeHeader(std::weak_ptr<CUISlateEditor> Editor)
    : m_Editor(Editor)
{

}

CUISlateNodeHeader::~CUISlateNodeHeader()
{
}



//
// CUISlateNodeHeader
//
void CUISlateNodeHeader::CreateDefault()
{
    SetSize(cDefaultBackgroundSize);

	m_Background = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
	m_Background->SetParentInternal(weak_from_this());
	m_Background->SetColor(cDefaultBackgroundColor);

	m_IconNode = std::make_shared<CUITextureNode>(vec2(cDefaultIconSize, cDefaultIconSize));
	m_IconNode->SetParentInternal(weak_from_this());
	m_IconNode->SetTexture(_RenderDevice->CreateTexture2D("Textures\\Operations\\LogMessage.png"));
	m_IconNode->SetTranslate(cDefaultIconOffset);

	vec2 labelPosition = m_IconNode->GetTranslation();
	labelPosition.x += cDefaultIconSize;
	labelPosition += cDefaultTextOffset;

	m_Text = std::make_shared<CUITextNode>();
	m_Text->SetParentInternal(weak_from_this());
	m_Text->SetText(cDefaultText);
	m_Text->SetTranslate(labelPosition);
	m_Text->SetTextColor(cDefaultTextColor);
	m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}


//
// CUIBaseNode
//

std::vector<std::shared_ptr<CUIBaseNode>> CUISlateNodeHeader::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    if (m_IconNode)
        childs.push_back(m_IconNode);

    if (m_Text)
        childs.push_back(m_Text);

    return childs;
}



//
// Input events
//
bool CUISlateNodeHeader::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    std::shared_ptr<CUISlateEditor> editor = m_Editor.lock();
    _ASSERT(editor);

    editor->BeginMoveNode(std::dynamic_pointer_cast<CUISlateNode>(GetParent()), e.GetPoint() - GetParent()->GetTranslationAbs());

    return true;
}

void CUISlateNodeHeader::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
}
