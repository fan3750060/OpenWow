#include "stdafx.h"

// Include
#include "UI/Fonts/UI_Font_Material.h"

// General
#include "UIText.h"

UIText::UIText()
{
	m_Font = GetManager<IFontsManager>()->GetMainFont();
	m_Text = "<empty>";

	m_Material = std::make_shared<UI_Font_Material>();
	m_Material->SetWrapper(m_Material);
	m_Material->SetTexture(0, m_Font->GetTexture());

	
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

void UIText::SetColor(cvec4 _color)
{
	m_Material->SetColor(_color);
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
	m_Font->SetMaterial(m_Material);

	vec2 _offset = vec2(0.0f, 0.0f);

	for (uint32 i = 0; i < m_Text.length(); i++)
	{
		uint8 ch = m_Text.c_str()[i];
		m_Material->SetOffset(_offset);
		_offset.x += static_cast<float>(m_Font->GetCharWidth(ch));

		m_Font->Render(renderEventArgs, m_PerObjectConstantBuffer, 0, 0, (ch) * 6, 6);
	}

	return true;
}
