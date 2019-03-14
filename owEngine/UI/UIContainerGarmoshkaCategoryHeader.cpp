#include "stdafx.h"

// General
#include "UIContainerGarmoshkaCategoryHeader.h"

// Additional
#include "UIUtils.h"

namespace
{
    // Background
    const vec2  cDefaultBackgroundSize = vec2(240.0f, 20.0f);
    const vec4  cDefaultBackgroundColor = vec4(0.23f, 0.7f, 0.23f, 1.0f);

    // Text
    const char* cDefaultText = "Category header";
    const uint32 cDefaultTextHeight = 12;
    const vec2  cDefaultTextOffset = vec2(3.0f, 3.0f);
    const vec4  cDefaultTextColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}


CUIContainerGarmoshkaCategoryHeader::CUIContainerGarmoshkaCategoryHeader()
{
}

CUIContainerGarmoshkaCategoryHeader::~CUIContainerGarmoshkaCategoryHeader()
{
}



//
// CUIContainerGarmoshkaCategoryHeader
//
void CUIContainerGarmoshkaCategoryHeader::CreateDefault()
{
    m_Background = std::make_shared<CUIColorNode>(cDefaultBackgroundSize);
    m_Background->SetParentInternal(weak_from_this());
    m_Background->SetColor(cDefaultBackgroundColor);

    m_Text = std::make_shared<CUITextNode>();
    m_Text->SetParentInternal(weak_from_this());
    m_Text->SetText(cDefaultText);
    m_Text->SetTextColor(cDefaultTextColor);
    m_Text->SetOffset(cDefaultTextOffset);
    m_Text->SetFont(GetManager<IFontsManager>()->Add("Fonts\\JustBreatheBd.otf", cDefaultTextHeight));
}

void CUIContainerGarmoshkaCategoryHeader::SetText(cstring Text)
{
    m_Text->SetText(Text);
}



//
// CUIBaseNode
//
glm::vec2 CUIContainerGarmoshkaCategoryHeader::GetSize() const
{
    return m_Background->GetSize();
}

std::vector<std::shared_ptr<CUIBaseNode>> CUIContainerGarmoshkaCategoryHeader::GetChilds() const
{
    std::vector<std::shared_ptr<CUIBaseNode>> childs;

    if (m_Background)
        childs.push_back(m_Background);

    if (m_Text)
        childs.push_back(m_Text);

    return childs;
}

bool CUIContainerGarmoshkaCategoryHeader::OnMouseButtonPressed(MouseButtonEventArgs & e)
{
    return false;
}

void CUIContainerGarmoshkaCategoryHeader::OnMouseButtonReleased(MouseButtonEventArgs & e)
{
}
