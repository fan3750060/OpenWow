#include "stdafx.h"

// General
#include "UIText.h"

UIText::UIText()
{
	m_Font = GetManager<IFontsManager>()->GetMainFont();
	m_Text = "<empty>";
}

UIText::~UIText()
{}



//
// UIText
//
void UIText::SetFont(std::shared_ptr<CFontMesh> _font)
{
	m_Font = _font;
}

void UIText::SetText(const std::string& _text)
{
	m_Text = _text;
}



//
// SceneNodeUI
//
void UIText::SetMesh(std::shared_ptr<IMesh> mesh)
{
	fail1();
}
std::shared_ptr<IMesh> UIText::GetMesh() const
{
	fail1();
	return nullptr;
}

bool UIText::RenderMesh(RenderUIEventArgs& renderEventArgs)
{
	return m_Font->Render(renderEventArgs, m_PerObjectConstantBuffer, m_Text);
}
