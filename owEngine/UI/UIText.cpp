#include "stdafx.h"

// General
#include "UIText.h"

UIText::UIText()
{
	m_Font = GetManager<IFontsManager>()->GetMainFont();
	m_Text = "<empty>";
}

UIText::~UIText()
{
}



void UIText::SetFont(std::shared_ptr<Font> _font)
{
	SetMesh(_font);
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
	//m_Font = mesh;
}
std::shared_ptr<IMesh> UIText::GetMesh() const
{
	return m_Font;
}

bool UIText::RenderMesh(RenderUIEventArgs& renderEventArgs)
{
	return m_Font->Render(renderEventArgs, m_PerObjectConstantBuffer, m_Text);
}
